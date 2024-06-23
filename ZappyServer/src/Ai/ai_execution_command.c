/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ai_execution_command
*/

#include <zappy_server.h>

int start_command(client_t *client)
{
    struct timeval start;

    if (client == NULL)
        return ERROR;
    if (client->commands[0].command == NULL)
        return OK;
    gettimeofday(&start, NULL);
    client->commands[0].start_time = start.tv_sec + start.tv_usec / 1000000.0;
    client->commands[0].is_started = true;
    return OK;
}

static bool check_condition(zappy_server_t *zappy, client_t *client)
{
    if (client == NULL || zappy == NULL)
        return false;
    if (client->commands[0].condition != NULL &&
        client->commands[0].condition(zappy, client) == false){
            dprintf(zappy->actual_sockfd, "ko\n");
            unqueue_command(client);
            return false;
        }
    return true;
}

static int exec_command_sub(zappy_server_t *zappy, client_t *client)
{
    struct timeval end;
    double elapsed;

    if (client == NULL || zappy == NULL)
        return ERROR;
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec + end.tv_usec / 1000000.0) -
        client->commands[0].start_time;
    if (client->commands[0].is_started == true &&
        client->commands[0].func != NULL &&
        client->commands[0].cast_time < elapsed) {
            if (check_condition(zappy, client) == false)
                return OK;
        client->commands[0].func(zappy, client, client->commands[0].command);
        unqueue_command(client);
    }
    return OK;
}

int exec_command(zappy_server_t *zappy, client_t *client)
{
    if (client == NULL || zappy == NULL)
        return ERROR;
    if (client->commands[0].command == NULL)
        return OK;
    if (client->commands[0].is_started == false) {
        check_condition(zappy, client);
        if (client->commands[0].command == NULL)
            return OK;
        if (strcmp(client->commands[0].command, "NULL") == 0){
            dprintf(zappy->actual_sockfd, "ko\n");
            unqueue_command(client);
        }
        start_command(client);
    }
    if (exec_command_sub(zappy, client) == ERROR)
        return ERROR;
    return OK;
}
