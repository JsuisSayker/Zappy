/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** scan_fd
*/

#include "zappy_server.h"

int scan_fd(zappy_server_t *zappy)
{
    for (zappy->actual_sockfd = 0; zappy->actual_sockfd < FD_SETSIZE;
        zappy->actual_sockfd += 1) {
        if (fd_is_set(zappy) == ERROR)
            return ERROR;
    }
    return OK;
}
