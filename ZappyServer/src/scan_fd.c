/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** scan_fd
*/

#include "zappy_server.h"

int scan_fd(zappy_server_t *zappy_server)
{
    for (zappy_server->actual_sockfd = 0;
        zappy_server->actual_sockfd < __FD_SETSIZE;
        zappy_server->actual_sockfd += 1) {
        if (fd_is_set(zappy_server) == ERROR)
            return ERROR;
    }
    return OK;
}
