/*
** EPITECH PROJECT, 2024
** server_functions
** File description:
** server_functions
*/

#include "zappy_server.h"

int type_gui(zappy_server_t *zappy, char *cmd)
{
    if (zappy == NULL || cmd == NULL)
        return ERROR;
    zappy->clients[zappy->actual_sockfd].type = GUI;
    handle_gui_command(zappy, cmd);
    return OK;
}
