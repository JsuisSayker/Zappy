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
    client_t *client;

    if (zappy_server == NULL)
        return ERROR;
    client = &zappy_server->clients[zappy_server->actual_sockfd];
    if (FD_ISSET(zappy_server->actual_sockfd, &zappy_server->fd.input)) {
        if (check_connection(zappy_server) == ERROR)
            return ERROR;
        return OK;
    }
    if (client->command.execusion != NULL || (client->command.queue
        != NULL && client->command.queue[0] != NULL))
        if (handle_ai_command(zappy_server, client, NULL) != OK)
            return KO;
    return OK;
}
