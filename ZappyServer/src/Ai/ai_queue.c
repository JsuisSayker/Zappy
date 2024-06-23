/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** cast_action
*/

#include <zappy_server.h>

static void destroy_node(client_t *client, int index)
{
    if (client == NULL)
        return;
    free_string(&client->commands[index].command);
    client->commands[index].func = NULL;
    client->commands[index].condition = NULL;
    client->commands[index].cast_time = 0;
    client->commands[index].start_time = 0.0;
    client->commands[index].is_started = false;
}

int unqueue_command(client_t *client)
{
    if (client == NULL)
        return ERROR;
    destroy_node(client, 0);
    for (int i = 0; i < QUEUE_SIZE - 1; i++) {
        if (client->commands[i + 1].command != NULL) {
            free_string(&client->commands[i].command);
            client->commands[i].command =
                strdup(client->commands[i + 1].command);
            client->commands[i].func = client->commands[i + 1].func;
            client->commands[i].condition = client->commands[i + 1].condition;
            client->commands[i].cast_time = client->commands[i + 1].cast_time;
            client->commands[i].start_time =
                client->commands[i + 1].start_time;
            client->commands[i].is_started =
                client->commands[i + 1].is_started;
            }
    }
    if (client->commands[QUEUE_SIZE - 1].command != NULL)
        destroy_node(client, QUEUE_SIZE - 1);
    return OK;
}

static void add_in_queue_sub(ai_command_t *queue, char *command,
    list_command_ai_t info_cmd, double freq)
{
    if (queue == NULL)
        return;
    if (strcmp(info_cmd.command, "NULL") == 0)
        queue->command = strdup(info_cmd.command);
    else
        queue->command = strdup(command);
    if (queue->command == NULL)
        return;
    queue->func = info_cmd.func;
    queue->condition = info_cmd.condition;
    queue->cast_time = info_cmd.cast_time / freq;
    queue->start_time = 0.0;
    queue->is_started = false;
}

void add_in_queue(client_t *client, char *command, list_command_ai_t info_cmd,
    double freq)
{
    if (client == NULL)
        return;
    for (int i = 0; i < QUEUE_SIZE; i ++) {
        if (client->commands[i].command == NULL) {
            add_in_queue_sub(&client->commands[i], command, info_cmd, freq);
            return;
        }
    }
}
