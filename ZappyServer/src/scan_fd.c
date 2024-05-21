/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** scan_fd
*/

#include "zappy_server.h"

static int check_connection(zappy_server_t *zappy_server)
{
    int client_fd = 0;

    if (zappy_server->actual_sockfd == zappy_server->my_socket) {
        client_fd = accept_new_connection(zappy_server->my_socket);
        if (client_fd == ERROR)
            return ERROR;
        dprintf(client_fd, "220 Service ready for new user.\n");
        dprintf(client_fd, END_STR);
        FD_SET(client_fd, &zappy_server->fd.save_input);
    } else {
        handle_client(zappy_server);
    }
    return OK;
}

static int fd_is_set(zappy_server_t *zappy_server)
{
    if (FD_ISSET(zappy_server->actual_sockfd, &zappy_server->fd.input)) {
        if (check_connection(zappy_server) == ERROR) {
            return ERROR;
        }
        return OK;
    }
    return OK;
}

int scan_fd(zappy_server_t *zappy_server)
{
    for (zappy_server->actual_sockfd = 0;
        zappy_server->actual_sockfd < __FD_SETSIZE;
        zappy_server->actual_sockfd += 1) {
        if (fd_is_set(zappy_server) == ERROR) {
            return ERROR;
        }
    }
    return OK;
}
