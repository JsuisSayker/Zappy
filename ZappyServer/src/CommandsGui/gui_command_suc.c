/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

void gui_command_suc(zappy_server_t *zappy, UNUSED char *command)
{
    dprintf(zappy->actual_sockfd, "suc\n");
}
