/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

void server_command_quit(zappy_server_t *zappy, char *command)
{
    if (zappy == NULL || command == NULL || command[0] != '\0')
        return;
    close(zappy->actual_sockfd);
    FD_CLR(zappy->actual_sockfd, &zappy->fd.save_input);
    zappy->server_running = false;
}
