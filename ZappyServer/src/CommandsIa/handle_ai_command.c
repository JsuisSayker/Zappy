/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** handle_ai__command
*/

#include <zappy_server.h>
    // {"/avance", &avance},
    // {"/droite", &droite},
    // {"/gauche", &gauche},
    // {"/voir", &voir},
    // {"/inventai_re", &inventai_re},
    // {"/prend", &prend},
    // {"/pose", &pose},
    // {"/expulse", &expulse},
    // {"/broadcast", &broadcast},
    // {"/incantation", &incantation},
    // {"/fork", &fork_command},
    // {"/connect_nbr", &connect_nbr},

static const struct command_ai_s COMMAND_FUNCS[] = {
    {"Help", &ai_command_help},
    {"Forward", &ai_command_forward},
    {"NULL", NULL}
};

static int handle_exec_command(zappy_server_t *zappy, client_t *client)
{
    if (zappy == NULL || client == NULL)
        return ERROR;
    for (int i = 0; COMMAND_FUNCS[i].func != NULL; i += 1) {
        if (strncmp(client->command.execusion, COMMAND_FUNCS[i].command,
            strlen(COMMAND_FUNCS[i].command)) == 0) {
            COMMAND_FUNCS[i].func(zappy, client, client->command.execusion);
            return OK;
        }
    }
    return ERROR;
}

int handle_ai_command(zappy_server_t *zappy, client_t *client, char *command)
{
    if (zappy == NULL || client == NULL)
        return ERROR;
    if (queue_to_exec(client) != OK)
        return ERROR;
    if (client->command.execusion != NULL){
        if (command != NULL && add_in_queue(client, command) == ERROR)
            return ERROR;
        if (handle_exec_command(zappy, client) == OK)
            return OK;
        return OK;
    }
    for (int i = 0; COMMAND_FUNCS[i].func != NULL; i += 1) {
        if (strncmp(command, COMMAND_FUNCS[i].command,
            strlen(COMMAND_FUNCS[i].command)) == 0) {
            COMMAND_FUNCS[i].func(zappy, client, command);
            return OK;
        }
    }
    return ERROR;
}
