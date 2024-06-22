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
        printf("client->type = %d\n", client->type);
        handle_client(zappy);
    }
    return OK;
}

static void add_resource(char *resource, inventory_t *inventory)
{
    if (strcmp(resource, "food") == 0)
        inventory->food += 1;
    if (strcmp(resource, "linemate") == 0)
        inventory->linemate += 1;
    if (strcmp(resource, "deraumere") == 0)
        inventory->deraumere += 1;
    if (strcmp(resource, "sibur") == 0)
        inventory->sibur += 1;
    if (strcmp(resource, "mendiane") == 0)
        inventory->mendiane += 1;
    if (strcmp(resource, "phiras") == 0)
        inventory->phiras += 1;
    if (strcmp(resource, "thystame") == 0)
        inventory->thystame += 1;
}

static void refill_map(zappy_server_t *zappy)
{
    struct timeval tv;
    double cast_time = 20 / (double)zappy->args->freq;
    double elapsed = 0;
    int x = rand() % zappy->args->width;
    int y = rand() % zappy->args->height;
    struct char_tab_head *resourse_list = NULL;

    gettimeofday(&tv, NULL);
    elapsed = (tv.tv_sec + tv.tv_usec / 1000000.0) - zappy->time_refill_map;
    if (cast_time > elapsed) {
        resourse_list = generate_ressourse_list(zappy->args->width,
            zappy->args->height);
        add_resource(resourse_list->tqh_first->str,
                &zappy->map_tile[y][x].inventory);
        free_char_tab_list(resourse_list);
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
    if (zappy->server_start_game) {
        if ((client->command.execution != NULL ||
        (client->command.queue != NULL && client->command.queue[0] != NULL))
        && (ai_function(zappy, client, NULL) != OK))
            return KO;
        if (client->type == IA)
            is_alive(zappy, client);
        refill_map(zappy);
    }
    return OK;
}
