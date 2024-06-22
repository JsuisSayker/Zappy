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
            client->pos.direction = EAST;
            break;
        case SOUTH:
            client->pos.direction = WEST;
            break;
        case EAST:
            client->pos.direction = SOUTH;
            break;
        case WEST:
            client->pos.direction = NORTH;
            break;
        default:
            return;
    }
    dprintf(zappy->actual_sockfd, "ok\n");
    send_ppo_command_to_all_gui(zappy, client);
}

int ai_command_right(zappy_server_t *zappy, client_t *client, char *cmd)
{
    printf("Right\n--\n");
    if (client == NULL || zappy == NULL || cmd == NULL)
        return ERROR;
    check_direction(zappy, client);
    return OK;
}
