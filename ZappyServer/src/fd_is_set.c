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
            &zappy_server->clients[zappy_server->actual_sockfd]
                 .other_socket_addr);
        if (client_fd == ERROR)
            return ERROR;
        dprintf(client_fd, "WELCOME\n");
        FD_SET(client_fd, &zappy_server->fd.save_input);
    } else {
        handle_client(zappy_server);
    }
    return OK;
}

static void send_map_info_first_gui(zappy_server_t *zappy_server)
{
    for (int i = 0; i < FD_SETSIZE; i++) {
        if (zappy_server->clients[i].type == GUI) {
            send_gui_map_content(zappy_server->map_tile,
            zappy_server->args->width, zappy_server->args->height, i);
            return;
        }
    }
}

static void refill_map(zappy_server_t *zappy_server, client_t *client)
{
    struct timeval tv;
    double cast_time = 20 / (double)zappy_server->args->freq;
    double elapsed = 0;

    gettimeofday(&tv, NULL);
    elapsed = (tv.tv_sec + tv.tv_usec / 1000000.0) - client->command.time;
    if (cast_time < elapsed) {
        refill_map_tile(zappy_server->map_tile, zappy_server->map_tile_save);
        zappy_server->time_refill_map = time(NULL);
        // send_map_info_first_gui(zappy_server);
    }
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
    if (client->command.execution != NULL ||
        (client->command.queue != NULL && client->command.queue[0] != NULL)) {
        if (ai_function(zappy_server, client, NULL) != OK)
            return KO;
    }
    if (client->type == IA)
        is_alive(zappy_server, client);
    refill_map(zappy_server, client);
    return OK;
}
