/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** handle_ai__command
*/

#include <zappy_server.h>

const list_command_ai_t COMMAND_FUNCS[] = {
    {"Forward", 7.0, &ai_command_forward, NULL},
    {"Right", 7.0, &ai_command_right, NULL},
    {"Left", 7.0, &ai_command_left, NULL},
    {"Look", 7.0, &ai_command_look, NULL},
    {"Inventory", 1.0, &ai_command_inventory, NULL},
    {"Broadcast", 7.0, &ai_command_breadcast, NULL},
    {"Fork", 42.0, &ai_command_fork, NULL},
    {"Eject", 7.0, &ai_command_eject, NULL},
    {"Take", 7.0, &ai_command_take_object, NULL},
    {"Set", 7.0, &ai_command_set, NULL},
    {"Incantation", 300.0, &ai_command_incantation, incantation_condition},
    {"Connect_nbr", 0.0, &ai_command_connect_nbr, NULL},
    {"NULL", 0.0, NULL, NULL}
};

void handle_command_in_queue(client_t *client, char *command, int freq)
{
    int i = 0;

    if (client == NULL || command == NULL)
        return;
    for (; COMMAND_FUNCS[i].command != NULL; i++) {
        if (strncmp(command, COMMAND_FUNCS[i].command,
            strlen(COMMAND_FUNCS[i].command)) == 0) {
            add_in_queue(client, command, COMMAND_FUNCS[i], freq);
            return;
        }
    }
    add_in_queue(client, command, COMMAND_FUNCS[i], freq);
}

int handle_ai_command(zappy_server_t *zappy, client_t *client, char *command)
{
    if (client == NULL || zappy == NULL)
        return ERROR;
    if (command == NULL)
        return OK;
    handle_command_in_queue(client, command, zappy->args->freq);
    return OK;
}
