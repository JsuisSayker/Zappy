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
    struct timeval start;

    if (client == NULL || zappy == NULL || cmd == NULL)
        return ERROR;
    if (client->command.is_contracted == false) {
        printf("Cast action\n cast_time: %d\n command: %s\n zappy.freq: %d\n--\n", cast_time, cmd, zappy->args->freq);
        client->command.is_contracted = true;
        client->command.cast_time = (double)cast_time
            / (double)zappy->args->freq;
        client->command.execusion = strdup(cmd);
        gettimeofday(&start, NULL);
        client->command.time = start.tv_sec + start.tv_usec / 1000000.0;
    }
    return OK;
}

bool check_action(zappy_server_t *zappy, client_t *client)
{
    struct timeval end;
    double elapsed;

    if (client == NULL || zappy == NULL)
        return false;
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec + end.tv_usec / 1000000.0) - client->command.time;
    printf("Check action\n elapsed: %lf\n cast_time: %f\n--\n", elapsed, client->command.cast_time);
    if (client->command.is_contracted == true &&
    client->command.cast_time < elapsed) {
        client->command.is_contracted = false;
        client->command.cast_time = 0;
        client->command.time = 0.0;
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
    printf("Queue to exec sub\n client.cmd: %s\n--\n", client->command.execusion);
    if (client->command.execusion == NULL)
        return ERROR;
    for (int i = 0; i < 10; i += 1){
        if (client->command.queue[i] != NULL){
            free(client->command.queue[i]);
            client->command.queue[i] = NULL;
        }
        if (client->command.queue[i + 1] != NULL)
            client->command.queue[i] = strdup(client->command.queue[i + 1]);
    }
    client->command.queue[9] = NULL;
    return OK;
}

int queue_to_exec(client_t *client)
{
    printf("Queue to exec\n");
    if (client == NULL)
        return ERROR;
    if (client->command.queue[0] != NULL) {
        printf(" add in exec\n");
        printf(" client.cmd: %s\n--\n", client->command.queue[0]);
        printf(" client.cmd: %s\n--\n", client->command.execusion);
        if (client->command.execusion != NULL)
            return OK;
        if (queue_to_exec_sub(client) == ERROR)
            return ERROR;
    }
    printf("Queue to exec end\n--\n");
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
    printf("Add in queue\n cmd: %s\n", cmd);
    for (i = 0; i < 10; i += 1){
        if (client->command.queue[i] == NULL){
            client->command.queue[i] = strdup(cmd);
            printf(" client.cmd: %s\n--\n", client->command.queue[i]);
            break;
        }
    }
    return OK;
}
