/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** handle_ai__command
*/

#include <zappy_server.h>
    // {"/voir", &voir},
    // {"/prend", &prend},
    // {"/pose", &pose},
    // {"/expulse", &expulse},
    // {"/broadcast", &broadcast},
    // {"/incantation", &incantation},
    // {"/fork", &fork_command},
    // {"/connect_nbr", &connect_nbr},

static const struct command_ai_s COMMAND_FUNCS[] = {
    {"Forward", &ai_command_forward},
    {"Right", &ai_command_right},
    {"Left", &ai_command_left},
    {"Take", &ai_command_take_object},
    {"Fork", &ai_command_fork},
    {"Inventory", &ai_command_inventory},
    {"Look", &ai_command_look},
    {"Connect_nbr", &ai_command_connect_nbr},
    {"NULL", NULL}
};

static int handle_ai_command_sub(zappy_server_t *zappy, client_t *client,
    char *cmd)
{
    if (zappy == NULL || client == NULL)
        return ERROR;
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
    if (zappy == NULL || client == NULL)
        return ERROR;
    if (queue_to_exec(client) != OK)
        return ERROR;
    if (client->command.execution != NULL){
        if (command != NULL && add_in_queue(client, command) == ERROR)
            return ERROR;
        if (handle_ai_command_sub(zappy, client, client->command.execution)
            != OK)
            return ERROR;
        return OK;
    }
    if (handle_ai_command_sub(zappy, client, command) != OK)
        return ERROR;
    return ERROR;
}
