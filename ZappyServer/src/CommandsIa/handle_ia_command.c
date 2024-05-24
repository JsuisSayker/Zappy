/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** handle_ia_command
*/

#include <zappy_server.h>
    // {"/avance", &avance},
    // {"/droite", &droite},
    // {"/gauche", &gauche},
    // {"/voir", &voir},
    // {"/inventaire", &inventaire},
    // {"/prend", &prend},
    // {"/pose", &pose},
    // {"/expulse", &expulse},
    // {"/broadcast", &broadcast},
    // {"/incantation", &incantation},
    // {"/fork", &fork_command},
    // {"/connect_nbr", &connect_nbr},

static const struct command_ia_s COMMAND_FUNCS[] = {
    {"Help", &ia_command_help},
    {"NULL", NULL}
};

int handle_ia_command(zappy_server_t *zappy, client_t *client, char *command)
{
    for (int i = 0; COMMAND_FUNCS[i].func != NULL; i += 1) {
        if (strncmp(command, COMMAND_FUNCS[i].command,
            strlen(COMMAND_FUNCS[i].command)) == 0) {
            COMMAND_FUNCS[i].func(zappy, client,
                &command[strlen(COMMAND_FUNCS[i].command)]);
            return OK;
        }
    }
    return ERROR;
}
