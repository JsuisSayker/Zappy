/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ai_receive_breadcast
*/

#include "zappy_server.h"


static int calculate_displacement(int pos1, int pos2, int max)
{
    int direct = pos2 - pos1;
    int wrap_around = (direct > 0) ? direct - max : direct + max;

    return (abs(direct) < abs(wrap_around)) ? direct : wrap_around;
}

static ai_position_t shortest_vector(ai_position_t sender,
    ai_position_t recever, int map_size_x, int map_size_y)
{
    ai_position_t vector;

    if (sender.x == recever.x && sender.y == recever.y) {
        vector.x = 0;
        vector.y = 0;
        return vector;
    }
    vector.x = calculate_displacement(sender.x, recever.x, map_size_x);
    vector.y = calculate_displacement(sender.y, recever.y, map_size_y);
    return vector;
}

static int position_east(ai_position_t block)
{
    if (block.x == 1 && block.y == 0)
        return 1;
    if (block.x == 1 && block.y == 1)
        return 2;
    if (block.x == 0 && block.y == 1)
        return 3;
    if (block.x == -1 && block.y == 1)
        return 4;
    if (block.x == -1 && block.y == 0)
        return 5;
    if (block.x == -1 && block.y == -1)
        return 6;
    if (block.x == 0 && block.y == -1)
        return 7;
    if (block.x == 1 && block.y == -1)
        return 8;
    return 0;
}

static int position_north(ai_position_t block)
{
    if (block.x == 0 && block.y == 1)
        return 1;
    if (block.x == -1 && block.y == 1)
        return 2;
    if (block.x == -1 && block.y == 0)
        return 3;
    if (block.x == -1 && block.y == -1)
        return 4;
    if (block.x == 0 && block.y == -1)
        return 5;
    if (block.x == 1 && block.y == -1)
        return 6;
    if (block.x == 1 && block.y == 0)
        return 7;
    if (block.x == 1 && block.y == 1)
        return 8;
    return 0;
}

static int position_south(ai_position_t block)
{
    if (block.x == 0 && block.y == -1)
        return 1;
    if (block.x == 1 && block.y == -1)
        return 2;
    if (block.x == 1 && block.y == 0)
        return 3;
    if (block.x == 1 && block.y == 1)
        return 4;
    if (block.x == 0 && block.y == 1)
        return 5;
    if (block.x == -1 && block.y == 1)
        return 6;
    if (block.x == -1 && block.y == 0)
        return 7;
    if (block.x == -1 && block.y == -1)
        return 8;
    return 0;
}

static int position_west(ai_position_t block)
{
    if (block.x == -1 && block.y == 0)
        return 1;
    if (block.x == -1 && block.y == -1)
        return 2;
    if (block.x == 0 && block.y == -1)
        return 3;
    if (block.x == 1 && block.y == -1)
        return 4;
    if (block.x == 1 && block.y == 0)
        return 5;
    if (block.x == 1 && block.y == 1)
        return 6;
    if (block.x == 0 && block.y == 1)
        return 7;
    if (block.x == -1 && block.y == 1)
        return 8;
    return 0;
}

static int position_got(ai_position_t relative, ai_position_t recever)
{
    ai_position_t block;

    block.x = relative.x - recever.x;
    block.y = relative.y - recever.y;
    switch (recever.direction) {
        case EAST:
            return position_east(block);
        case NORTH:
            return position_north(block);
        case SOUTH:
            return position_south(block);
        case WEST:
            return position_west(block);
        default:
            return 0;
    }
}

ai_position_t message_path(ai_position_t recever, ai_position_t vector,
    int max_x, int max_y)
{
    ai_position_t path = {recever.x, recever.y, 0};

    if (vector.x > 0)
        (path.x)++;
    if (vector.x < 0) {
        (path.x)--;
        if (path.x < 0)
            path.x = max_x - 1;
    }
    if (vector.y > 0)
        (path.y)++;
    if (vector.y < 0) {
        (path.y)--;
        if (path.y < 0)
            path.y = max_y - 1;
    }
    return path;
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
    relative = message_path(client->pos, vector, zappy->args->width,
            zappy->args->height);
    i = position_got(relative, client->pos);
    dprintf(actual_sockfd, "message %d, %s\n", i, message->message);
    return OK;
}

int read_message_receive(zappy_server_t *zappy, client_t *client,
    message_t *message, int actual_sockfd)
{
    if (client == NULL || zappy == NULL || message == NULL)
        return ERROR;
    if (message_receive(client, zappy, message, actual_sockfd) == ERROR)
        return ERROR;
    return OK;
}
