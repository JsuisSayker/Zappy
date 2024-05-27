/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

void server_command_quit(UNUSED zappy_server_t *zappy, UNUSED char *command)
{
    close(zappy->actual_sockfd);
    FD_CLR(zappy->actual_sockfd, &zappy->fd.save_input);
    zappy->server_running = false;
}
