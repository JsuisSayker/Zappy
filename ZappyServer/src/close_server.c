/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** close_server
*/

#include <zappy_server.h>

static int send_logout_to_all_clients(zappy_server_t *zappy_server)
{
    for (zappy_server->actual_sockfd = 0;
        zappy_server->actual_sockfd < __FD_SETSIZE;
        zappy_server->actual_sockfd += 1) {
        if (fd_is_set(zappy_server) == ERROR)
            return ERROR;
    }
    return OK;
}

int close_server(zappy_server_t *zappy_server)
{
    send_logout_to_all_clients(zappy_server);
    close(zappy_server->my_socket);
    free_users(&(zappy_server->all_user));
    free_subscribed(&(zappy_server->subscribed_teams_users));
    free_teams(&zappy_server->all_teams);
    free_map_tile(zappy_server->map_tile);
    free_args_config(zappy_server->args);
    free(zappy_server);
    return OK;
}