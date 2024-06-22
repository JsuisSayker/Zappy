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

    vector.x = p1.x - p2.x;
    vector.y = p1.y - p2.y;
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
    if (vector.x == -1 && vector.y == -1)
        return 6;
    if (vector.x == 0 && vector.y == -1)
        return 7;
    if (vector.x == 1 && vector.y == -1)
        return 8;
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
    if (vector.x == 1 && vector.y == 0)
        return 7;
    if (vector.x == 1 && vector.y == 1)
        return 8;
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
    if (vector.x == -1 && vector.y == 0)
        return 7;
    if (vector.x == -1 && vector.y == -1)
        return 8;
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
    if (vector.x == 1 && vector.y == 1)
        return 6;
    if (vector.x == 0 && vector.y == 1)
        return 7;
    if (vector.x == -1 && vector.y == 1)
        return 8;
    return 0;
}

static int position_got(ai_position_t relactive, ai_position_t ai_pos,
    ai_direction_t direction)
{
    ai_position_t vector;

    vector.x = relactive.x - ai_pos.x;
    vector.y = relactive.y - ai_pos.y;
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
    message_t *message, int actual_sockfd)
{
    ai_position_t vector;
    ai_position_t relative;
    int i = 0;

    if (client == NULL || zappy == NULL || message == NULL)
        return ERROR;
    vector = shortest_vector(message->pos, client->pos, zappy->args->width,
            zappy->args->height);
    relative = relative_case(client->pos, vector, zappy->args->width,
            zappy->args->height);
    i = position_got(relative, message->pos, client->pos.direction);
    dprintf(actual_sockfd, "message %d, %s\n", i, message->message);
    return OK;
}

int read_message_receive(zappy_server_t *zappy, client_t *client,
    message_t *message, int actual_sockfd)
{
    printf("Receive message\n--\n");
    if (client == NULL || zappy == NULL || message == NULL)
        return ERROR;
    if (message_receive(client, zappy, message, actual_sockfd) == ERROR)
        return ERROR;
    return OK;
}
