/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** close_server
*/

#include <zappy_server.h>

static int send_logout_to_all_clients(zappy_server_t *zappy)
{
    for (zappy->actual_sockfd = 0; zappy->actual_sockfd < FD_SETSIZE;
        zappy->actual_sockfd += 1) {
        if (zappy->clients[zappy->actual_sockfd].team_name)
            free(zappy->clients[zappy->actual_sockfd].team_name);
    }
    return OK;
}

int close_server(zappy_server_t *zappy)
{
    send_logout_to_all_clients(zappy);
    close(zappy->my_socket);
    free_teams(&zappy->all_teams);
    free_map_tile(zappy->map_tile);
    free_args_config(zappy->args);
    free(zappy);
    printf("Server shutting down.\n");
    return OK;
}
