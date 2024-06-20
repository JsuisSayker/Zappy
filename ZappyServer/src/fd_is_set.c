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
    if (zappy->clients[zappy->actual_sockfd].type  > 3)
        printf("zappy->clients[zappy->actual_sockfd].type = %d\n",
            zappy->clients[zappy->actual_sockfd].type);
    client = &zappy->clients[zappy->actual_sockfd];
    if (client->type > 3)
        printf("client->type = %d\n", client->type);
    if (zappy->actual_sockfd == zappy->my_socket) {
        client_fd = accept_new_connection(zappy->my_socket,
            &zappy->clients[zappy->actual_sockfd].other_socket_addr);
        if (client_fd == ERROR)
            return ERROR;
        dprintf(client_fd, "WELCOME\n");
        FD_SET(client_fd, &zappy->fd.save_input);
        zappy->nb_connected_clients += 1;
    } else {
        printf("client->type = %d\n", client->type);
        handle_client(zappy);
    }
    return OK;
}

static void refill_map(zappy_server_t *zappy)
{
    struct timeval tv;
    double cast_time = 20 / (double)zappy->args->freq;
    double elapsed = 0;
    int x = rand() % zappy->args->width;
    int y = rand() % zappy->args->height;

    gettimeofday(&tv, NULL);
    elapsed = (tv.tv_sec + tv.tv_usec / 1000000.0) - zappy->time_refill_map;
    if (cast_time < elapsed) {
        struct char_tab_head *resourse_list = generate_ressourse_list(
            zappy->args->width, zappy->args->height);
        if (strcmp(resourse_list->tqh_first->str, "food") == 0)
            zappy->map_tile[y][x].inventory.food += 1;
        if (strcmp(resourse_list->tqh_first->str, "linemate") == 0)
            zappy->map_tile[y][x].inventory.linemate += 1;
        if (strcmp(resourse_list->tqh_first->str, "deraumere") == 0)
            zappy->map_tile[y][x].inventory.deraumere += 1;
        if (strcmp(resourse_list->tqh_first->str, "sibur") == 0)
            zappy->map_tile[y][x].inventory.sibur += 1;
        if (strcmp(resourse_list->tqh_first->str, "mendiane") == 0)
            zappy->map_tile[y][x].inventory.mendiane += 1;
        if (strcmp(resourse_list->tqh_first->str, "phiras") == 0)
            zappy->map_tile[y][x].inventory.phiras += 1;
        if (strcmp(resourse_list->tqh_first->str, "thystame") == 0)
            zappy->map_tile[y][x].inventory.thystame += 1;
        free_char_tab_list(resourse_list);
        zappy->time_refill_map = time(NULL);
    }
}

int fd_is_set(zappy_server_t *zappy)
{
    client_t *client;

    if (zappy == NULL)
        return ERROR;
    if (zappy->clients[zappy->actual_sockfd].type  > 3)
        printf("zappy->clients[zappy->actual_sockfd].type = %d\n",
            zappy->clients[zappy->actual_sockfd].type);
    client = &zappy->clients[zappy->actual_sockfd];
    if (client->type > 3)
        printf("client->type = %d\n", client->type);
    if (FD_ISSET(zappy->actual_sockfd, &zappy->fd.input)) {
        if (check_connection(zappy) == ERROR)
            return ERROR;
        return OK;
    }
    if (zappy->server_start_game) {
        if ((client->command.execution != NULL ||
        (client->command.queue != NULL && client->command.queue[0] != NULL))
        && (ai_function(zappy, client, NULL) != OK)) {
            return KO;
        }
        if (client->type == IA)
            is_alive(zappy, client);
        refill_map(zappy);
    }
    return OK;
}
