/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** handle_ai__command
*/

#include <zappy_server.h>
    // {"/expulse", &expulse},

static const struct command_ai_s COMMAND_FUNCS[] = {
    {"Forward", &ai_command_forward},
    {"Right", &ai_command_right},
    {"Left", &ai_command_left},
    {"Take", &ai_command_take_object},
    {"Set", &ai_command_set},
    {"Fork", &ai_command_fork},
    {"Inventory", &ai_command_inventory},
    {"Look", &ai_command_look},
    {"Connect_nbr", &ai_command_connect_nbr},
    {"Incantation", &ai_command_incantation},
    {"Broadcast", &ai_command_breadcast},
    {"Eject", &ai_command_eject},
    {"NULL", NULL}
};

static int handle_ai_command_sub(zappy_server_t *zappy, client_t *client,
    char *cmd)
{
    if (zappy == NULL || client == NULL)
        return ERROR;
    printf("--\n");
    for (int i = 0; COMMAND_FUNCS[i].func != NULL; i += 1) {
        if (strncmp(cmd, COMMAND_FUNCS[i].command,
            strlen(COMMAND_FUNCS[i].command)) == 0) {
            COMMAND_FUNCS[i].func(zappy, client, cmd);
            return OK;
        }
    }
    dprintf(zappy->actual_sockfd, "ko\n");
    if (client->command.execution != NULL){
        free(client->command.execution);
        client->command.execution = NULL;
    }
    return ERROR;
}

int handle_ai_command(zappy_server_t *zappy, client_t *client, char *command)
{
    char *cmd;

    if (zappy == NULL || client == NULL)
        return ERROR;
    if (queue_to_exec(client) != OK)
        return ERROR;
    if (client->command.execution != NULL){
        cmd = strdup(client->command.execution);
        if (command != NULL && add_in_queue(client, command) == ERROR)
            return ERROR;
        if (handle_ai_command_sub(zappy, client, cmd)
            != OK)
            return ERROR;
        return OK;
    }
    if (handle_ai_command_sub(zappy, client, command) != OK)
        return ERROR;
    return ERROR;
}
