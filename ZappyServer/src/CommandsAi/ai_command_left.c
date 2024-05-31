/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ai_command_right
*/

#include <zappy_server.h>

static void check_direction(zappy_server_t *zappy, client_t *client)
{
    switch (client->pos.direction) {
    case NORTH:
        client->pos.direction = WEST;
        dprintf(zappy->actual_sockfd, "ok\n");
        return;
    case SOUTH:
        client->pos.direction = EAST;
        dprintf(zappy->actual_sockfd, "ok\n");
        return;
    case EAST:
        client->pos.direction = NORTH;
        dprintf(zappy->actual_sockfd, "ok\n");
        return;
    case WEST:
        client->pos.direction = SOUTH;
        dprintf(zappy->actual_sockfd, "ok\n");
        return;
    default:
        return;
    }
}

int ai_command_left(zappy_server_t *zappy, client_t *client, char *cmd)
{
    if (client == NULL || zappy == NULL || cmd == NULL)
        return ERROR;
    if (cast_action(zappy, client, 7, cmd) == ERROR)
        return ERROR;
    printf("Left\n");
    if (check_action(zappy, client) == false)
        return OK;
    check_direction(zappy, client);
    return OK;
}
