/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** fd_is_set
*/

#include <zappy_server.h>

static int check_connection(zappy_server_t *zappy)
{
    int client_fd = 0;

    if (zappy->actual_sockfd == zappy->my_socket) {
        client_fd = accept_new_connection(zappy->my_socket,
            &zappy->clients[zappy->actual_sockfd]
            .other_socket_addr);
        if (client_fd == ERROR)
            return ERROR;
        dprintf(client_fd, "WELCOME\n");
        FD_SET(client_fd, &zappy->fd.save_input);
    } else {
        handle_client(zappy);
    }
    return OK;
}

static void refill_map(zappy_server_t *zappy, client_t *client)
{
    struct timeval tv;
    double cast_time = 20 / (double)zappy->args->freq;
    double elapsed = 0;

    gettimeofday(&tv, NULL);
    elapsed = (tv.tv_sec + tv.tv_usec / 1000000.0) - client->command.time;
    if (cast_time < elapsed) {
        refill_map_tile(zappy, zappy->map_tile,
            zappy->map_tile_save);
        zappy->time_refill_map = time(NULL);
    }
}

int fd_is_set(zappy_server_t *zappy)
{
    client_t *client;

    if (zappy == NULL)
        return ERROR;
    client = &zappy->clients[zappy->actual_sockfd];
    if (FD_ISSET(zappy->actual_sockfd, &zappy->fd.input)) {
        if (check_connection(zappy) == ERROR)
            return ERROR;
        return OK;
    }
    if (client->command.execution != NULL ||
        (client->command.queue != NULL && client->command.queue[0] != NULL)) {
        if (ai_function(zappy, client, NULL) != OK)
            return KO;
    }
    if (client->type == IA)
        is_alive(zappy, client);
    refill_map(zappy, client);
    return OK;
}
