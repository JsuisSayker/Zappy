/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** cast_action
*/

#include <zappy_server.h>
#include <sys/time.h>

int cast_action(zappy_server_t *zappy, client_t *client, int cast_time,
    char *cmd)
{
    if (client == NULL || zappy == NULL || cmd == NULL)
        return ERROR;
    if (client->command.is_contracted == false) {
        client->command.is_contracted = true;
        client->command.cast_time = cast_time / zappy->args->freq;
        printf("Client.cast time: %f\n", client->command.cast_time);
        printf("cast_time: %d\n", cast_time);
        client->command.execusion = strdup(cmd);
        printf("Command execute: %s\n", client->command.execusion);
        gettimeofday(&client->command.timeval, NULL);
    }
    return OK;
}

bool check_action(zappy_server_t *zappy, client_t *client)
{
    struct timeval end;
    double elapsed;

    printf("Check action\n");

    if (client == NULL || zappy == NULL)
        return false;
    gettimeofday(&end, NULL);
    elapsed = end.tv_sec - client->command.timeval.tv_sec;
    printf("Elapsed: %ld\n", client->command.timeval.tv_sec);
    if (client->command.is_contracted == true &&
    client->command.cast_time < elapsed) {
        client->command.is_contracted = false;
        client->command.cast_time = 0;
        client->command.timeval.tv_sec = 0;
        if (client->command.execusion != NULL){
            free(client->command.execusion);
            client->command.execusion = NULL;
        }
        return true;
    }
    return false;
}

static int queue_to_exec_sub(client_t *client)
{
    if (client == NULL)
        return ERROR;
    client->command.execusion = strdup(client->command.queue[0]);
    if (client->command.execusion == NULL)
        return ERROR;
    for (int i = 0; i < 10; i += 1){
        if (client->command.queue[i] != NULL)
            free(client->command.queue[i]);
        client->command.queue[i] = strdup(client->command.queue[i + 1]);
    }
    client->command.queue[9] = NULL;
    return OK;
}

int queue_to_exec(client_t *client)
{
    if (client == NULL)
        return ERROR;
    if (client->command.queue[0] != NULL) {
        if (client->command.execusion != NULL)
            free(client->command.execusion);
        if (queue_to_exec_sub(client) == ERROR)
            return ERROR;
    }
    return OK;
}

int init_queue(client_t *client)
{
    if (client == NULL)
        return ERROR;
    client->command.queue = malloc(sizeof(char *) * 11);
    if (client->command.queue == NULL)
        return ERROR;
    client->command.queue[10] = NULL;
    for (int i = 0; i < 10; i += 1)
        client->command.queue[i] = NULL;
    return OK;
}

int add_in_queue(client_t *client, char *cmd)
{
    int i = 0;

    if (client == NULL || cmd == NULL)
        return ERROR;
    for (i = 0; i < 10; i += 1){
        if (client->command.queue[i] == NULL){
            client->command.queue[i] = strdup(cmd);
            break;
        }
    }
    return OK;
}
