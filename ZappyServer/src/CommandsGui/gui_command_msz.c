/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

void gui_command_msz(zappy_server_t *zappy, UNUSED char *command)
{
    dprintf(zappy->actual_sockfd, "msz %d %d\n", zappy->args->width,
        zappy->args->height);
}
