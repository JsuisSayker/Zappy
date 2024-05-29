/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** fd_is_set
*/

#include <zappy_server.h>

static int check_connection(zappy_server_t *zappy_server)
{
    int client_fd = 0;

    if (zappy_server->actual_sockfd == zappy_server->my_socket) {
        client_fd = accept_new_connection(zappy_server->my_socket,
            &zappy_server->clients[zappy_server->actual_sockfd].
            other_socket_addr);
        if (client_fd == ERROR)
            return ERROR;
        dprintf(client_fd, "WELCOME\n");
        FD_SET(client_fd, &zappy_server->fd.save_input);
    } else {
        handle_client(zappy_server);
    }
    return OK;
}

int fd_is_set(zappy_server_t *zappy_server)
{
    if (FD_ISSET(zappy_server->actual_sockfd, &zappy_server->fd.input)) {
        if (check_connection(zappy_server) == ERROR) {
            return ERROR;
        }
        return OK;
    } else if (zappy_server->clients[zappy_server->actual_sockfd].command.execusion != NULL)
        if (handle_ai_command(zappy_server,
        &zappy_server->clients[zappy_server->actual_sockfd],
        NULL)
        != OK)
            return KO;
    return OK;
}
