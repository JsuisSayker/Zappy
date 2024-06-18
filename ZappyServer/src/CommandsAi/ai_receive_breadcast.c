/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ai_receive_breadcast
*/

#include "zappy_server.h"

static ai_position_t shortest_vector(ai_position_t p1, ai_position_t p2,
    int map_size_x, int map_size_y)
{
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
    int map_size_x, int map_size_y)
{
    ai_position_t relative = p2;

    if (vector.x != 0) {
        relative.x -= (vector.x > 0) ? 1 : -1;
        if (relative.x < 0)
            relative.x += map_size_x;
        if (relative.x >= map_size_x)
            relative.x -= map_size_x;
    }
    if (vector.y != 0) {
        relative.y -= (vector.y > 0) ? 1 : -1;
        if (relative.y < 0)
            relative.y += map_size_y;
        if (relative.y >= map_size_y)
            relative.y -= map_size_y;
    }
    return relative;
}

static int position_east(ai_position_t vector)
{
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
    return 0;
}

static int position_north(ai_position_t vector)
{
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
    return 0;
}

static int position_south(ai_position_t vector)
{
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
    return 0;
}

static int position_west(ai_position_t vector)
{
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
    return 0;
}

static int position_got(ai_position_t relactive, ai_position_t ai_pos,
    ai_direction_t direction)
{
    ai_position_t vector;

    vector.x = relactive.x - ai_pos.x;
    vector.y = relactive.y - ai_pos.y;
    printf("Vector: (%d, %d)\n", vector.x, vector.y);
    switch (direction) {
        case EAST:
            return position_east(vector);
        case NORTH:
            return position_north(vector);
        case SOUTH:
            return position_south(vector);
        case WEST:
            return position_west(vector);
        default:
            return 0;
    }
}

static int message_receive(client_t *client, zappy_server_t *zappy,
    message_t *tmp, int actual_socket)
{
    message_t *tmp_remove = NULL;
    ai_position_t vector;
    ai_position_t relative;
    int i = 0;

    if (client == NULL || zappy == NULL || tmp == NULL)
        return ERROR;
    vector = shortest_vector(tmp->pos, client->pos, zappy->args->width,
            zappy->args->height);
    relative = relative_case(client->pos, vector, zappy->args->width,
            zappy->args->height);
    printf("--\nActual socket %d\n", actual_socket);
    i = position_got(relative, client->pos, client->pos.direction);
    printf("Position: %d Message %s\n", i, tmp->message);
    dprintf(actual_socket, "message %d, %s\n", i, tmp->message);
    tmp_remove = tmp;
    tmp = tmp->next;
    remove_first_node(tmp_remove);
    printf("OUI\n");
    return OK;
}

int read_message_recieve(zappy_server_t *zappy, client_t *client,
    int actual_socket)
{
    message_t *tmp = NULL;

    if (client == NULL)
        return ERROR;
    tmp = client->message_receive;
    if (tmp == NULL)
        return OK;
    while (tmp != NULL) {
        if (message_receive(client, zappy, tmp, actual_socket) == ERROR)
            return ERROR;
    }
    client->message_receive = tmp;
    return OK;
}
