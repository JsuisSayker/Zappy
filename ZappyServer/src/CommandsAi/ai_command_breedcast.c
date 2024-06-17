/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ai_command_breedcast
*/

#include <zappy_server.h>

static int spreads_breedcast(zappy_server_t *zappy, client_t *client,
    char *message)
{
    if (zappy == NULL || client == NULL)
        return ERROR;
    for (int i = 3; i < FD_SETSIZE; i ++){
        zappy->clients[i].message_receive =
            add_node_in_list(zappy->clients[i].message_receive, message,
                client->pos);
        if (zappy->clients[i].message_receive == NULL)
            return ERROR;
    }
    dprintf(zappy->actual_sockfd, "ok\n");
    return OK;
}

int ai_command_breedcast(zappy_server_t *zappy, client_t *client,
    char *cmd)
{
    char *message = NULL;

    if (client == NULL || zappy == NULL || cmd == NULL)
        return ERROR;
    if (cast_action(zappy, client, 7, cmd) == ERROR)
        return ERROR;
    if (check_action(zappy, client) == false)
        return OK;
    message = strtok(cmd, " ");
    if (spreads_breedcast(zappy, client, message) == ERROR) {
        dprintf(zappy->actual_sockfd, "ko\n");
        return ERROR;
    }
    return OK;
}

static ai_position_t shortest_vector(ai_position_t p1, ai_position_t p2,
    int map_size_x, int map_size_y) {
    ai_position_t vector;

    vector.x = p2.x - p1.x;
    vector.y = p2.y - p1.y;
    if (vector.x > map_size_x / 2) {
        vector.x -= map_size_x;
    } else if (vector.x < -map_size_x / 2) {
        vector.x += map_size_x;
    }
    if (vector.y > map_size_y / 2) {
        vector.y -= map_size_y;
    } else if (vector.y < -map_size_y / 2) {
        vector.y += map_size_y;
    }
    return vector;
}

static ai_position_t relative_case(ai_position_t p2, ai_position_t vector,
    int map_size_x, int map_size_y) {
    ai_position_t relative = p2;

    if (vector.x != 0) {
        relative.x -= (vector.x > 0) ? 1 : -1;
        if (relative.x < 0) relative.x += map_size_x;
        if (relative.x >= map_size_x) relative.x -= map_size_x;
    }
    if (vector.y != 0) {
        relative.y -= (vector.y > 0) ? 1 : -1;
        if (relative.y < 0) relative.y += map_size_y;
        if (relative.y >= map_size_y) relative.y -= map_size_y;
    }
    return relative;
}

static int relative_case(ai_position_t relactive, ai_position_t ai_pos,
    ai_direction_t direction) {
    ai_position_t vector;

    vector.x = relactive.x - ai_pos.x;
    vector.y = relactive.y - ai_pos.y;
    if (direction == EAST) {
        if (vector.x == 1 && vector.y == 0)
            return 1;
        if (vector.x == 1 && vector.y == 1)
            return 2;
        if (vector.x == 0 && vector.y == 1)
            return 3;
        if (vector.x == -1 && vector.y == 1)
            return 4;
        if (vector.x == -1 && vector.y == 0)
            return 5;
        if (vector.x == 1 && vector.y == -1)
            return 6;
    }
    if (direction == NORTH) {
        if (vector.x == 0 && vector.y == 1)
            return 1;
        if (vector.x == -1 && vector.y == 1)
            return 2;
        if (vector.x == -1 && vector.y == 0)
            return 3;
        if (vector.x == -1 && vector.y == -1)
            return 4;
        if (vector.x == 0 && vector.y == -1)
            return 5;
        if (vector.x == 1 && vector.y == -1)
            return 6;
    }
    if (direction == SOUTH) {
        if (vector.x == 0 && vector.y == -1)
            return 1;
        if (vector.x == 1 && vector.y == -1)
            return 2;
        if (vector.x == 1 && vector.y == 0)
            return 3;
        if (vector.x == 1 && vector.y == 1)
            return 4;
        if (vector.x == 0 && vector.y == 1)
            return 5;
        if (vector.x == -1 && vector.y == 1)
            return 6;
    }
    if (direction == WEST) {
        if (vector.x == -1 && vector.y == 0)
            return 1;
        if (vector.x == -1 && vector.y == -1)
            return 2;
        if (vector.x == 0 && vector.y == -1)
            return 3;
        if (vector.x == 1 && vector.y == -1)
            return 4;
        if (vector.x == 1 && vector.y == 0)
            return 5;
        if (vector.x == -1 && vector.y == 1)
            return 6;
    }
    return 0;
}

int read_message_recieve(zappy_server_t *zappy, client_t *client,
    int actual_socket)
{
    message_t *tmp = NULL;
    ai_position_t vector;
    ai_position_t relative;

    if (client == NULL)
        return ERROR;
    tmp = client->message_receive;
    if (tmp == NULL)
        return OK;
    while (tmp != NULL){
        vector = shortest_vector(tmp->pos, client->pos, zappy->args->width,
            zappy->args->height);
        relative = relative_case(client->pos, vector, zappy->args->width,
        printf("P1: (%d, %d)\n", client->pos.x, client->pos.y);
        printf("P2: (%d, %d)\n", tmp->pos.x, tmp->pos.y);
        printf("Le vecteur le plus court entre P1 et P2 est: (%d, %d)\n", vector.x, vector.y);
        printf("La case relative à P2 par laquelle le message va arriver est: (%d, %d)\n", relative.x, relative.y);
        position_got(client, relative);
    }
}
