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
    client_t *client = &zappy->clients[zappy->actual_sockfd];

    if (zappy->actual_sockfd == zappy->my_socket) {
        client_fd = accept_new_connection(zappy->my_socket,
            &zappy->clients[zappy->actual_sockfd].other_socket_addr);
        if (client_fd == ERROR)
            return ERROR;
        dprintf(client_fd, "WELCOME\n");
        FD_SET(client_fd, &zappy->fd.save_input);
        zappy->nb_connected_clients += 1;
    } else {
        handle_client(zappy);
    }
    return OK;
}

static void refill_map(zappy_server_t *zappy)
{
    struct timeval tv;
    double cast_time = 20 / (double)zappy->args->freq;
    double elapsed = 0;
    int resource_to_add = 0;

    gettimeofday(&tv, NULL);
    elapsed = (tv.tv_sec + tv.tv_usec / 1000000.0) - zappy->time_refill_map;
    if (cast_time < elapsed) {
        resource_to_add = (FOOD_DENSITY * zappy->args->width *
            zappy->args->height / 100) - zappy->all_resources[0];
        for (int i = 0; i < resource_to_add; i += 1) {
            zappy->map_tile[rand() % zappy->args->height]
                [rand() % zappy->args->width].inventory.food += 1;
            zappy->all_resources[0] += 1;
        }
        resource_to_add = (LINEMATE_DENSITY * zappy->args->width *
            zappy->args->height / 100) - zappy->all_resources[1];
        for (int i = 0; i < resource_to_add; i += 1) {
            zappy->map_tile[rand() % zappy->args->height]
                [rand() % zappy->args->width].inventory.linemate += 1;
            zappy->all_resources[1] += 1;
        }
        resource_to_add = (DERAUMERE_DENSITY * zappy->args->width *
            zappy->args->height / 100) - zappy->all_resources[2];
        for (int i = 0; i < resource_to_add; i += 1) {
            zappy->map_tile[rand() % zappy->args->height]
                [rand() % zappy->args->width].inventory.deraumere += 1;
            zappy->all_resources[2] += 1;
        }
        resource_to_add = (SIBUR_DENSITY * zappy->args->width *
            zappy->args->height / 100) - zappy->all_resources[3];
        for (int i = 0; i < resource_to_add; i += 1) {
            zappy->map_tile[rand() % zappy->args->height]
                [rand() % zappy->args->width].inventory.sibur += 1;
            zappy->all_resources[3] += 1;
        }
        resource_to_add = (MENDIANE_DENSITY * zappy->args->width *
            zappy->args->height / 100) - zappy->all_resources[4];
        for (int i = 0; i < resource_to_add; i += 1) {
            zappy->map_tile[rand() % zappy->args->height]
                [rand() % zappy->args->width].inventory.mendiane += 1;
            zappy->all_resources[4] += 1;
        }
        resource_to_add = (PHIRAS_DENSITY * zappy->args->width *
            zappy->args->height / 100) - zappy->all_resources[5];
        for (int i = 0; i < resource_to_add; i += 1) {
            zappy->map_tile[rand() % zappy->args->height]
                [rand() % zappy->args->width].inventory.phiras += 1;
            zappy->all_resources[5] += 1;
        }
        resource_to_add = (THYSTAME_DENSITY * zappy->args->width *
            zappy->args->height / 100) - zappy->all_resources[6];
        for (int i = 0; i < resource_to_add; i += 1) {
            zappy->map_tile[rand() % zappy->args->height]
                [rand() % zappy->args->width].inventory.phiras += 1;
            zappy->all_resources[6] += 1;
        }
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
    if (zappy->server_start_game && client->type == AI) {
        if ((client->command.execution != NULL ||
        (client->command.queue != NULL && client->command.queue[0] != NULL))
        && (ai_function(zappy, client, NULL) != OK))
            return KO;
    }
    refill_map(zappy);
    return OK;
}
