/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ai_command_forward
*/

#include <zappy_server.h>

static int check_direction_sub(zappy_server_t *zappy, client_t *client)
{
    if (client->pos.direction == EAST){
        if (client->pos.x == zappy->args->width - 1)
            client->pos.x = 0;
        else
            client->pos.x += 1;
        dprintf(zappy->actual_sockfd, "ok\n");
        send_ppo_command_to_all_gui(zappy, client);
        return OK;
    }
    if (client->pos.direction == WEST){
        if (client->pos.x == 0)
            client->pos.x = zappy->args->width - 1;
        else
            client->pos.x = client->pos.x - 1;
        dprintf(zappy->actual_sockfd, "ok\n");
        send_ppo_command_to_all_gui(zappy, client);
        return OK;
    }
    return ERROR;
}

static int check_direction(zappy_server_t *zappy, client_t *client)
{
    if (client->pos.direction == NORTH){
        if (client->pos.y == zappy->args->height - 1)
            client->pos.y = 0;
        else
            client->pos.y += 1;
        dprintf(zappy->actual_sockfd, "ok\n");
        return OK;
    }
    if (client->pos.direction == SOUTH){
        if (client->pos.y == 0)
            client->pos.y = zappy->args->height - 1;
        else
            client->pos.y = client->pos.y - 1;
        dprintf(zappy->actual_sockfd, "ok\n");
        return OK;
    }
    if (check_direction_sub(zappy, client) == ERROR)
        return ERROR;
    return OK;
}

int ai_command_forward(zappy_server_t *zappy, client_t *client, char *cmd)
{
    printf("    FORWARD\n");
    if (client == NULL || zappy == NULL || cmd == NULL)
        return ERROR;
    if (cast_action(zappy, client, 7, cmd) == ERROR)
        return ERROR;
    if (check_action(zappy, client) == false)
        return OK;
    if (check_direction(zappy, client) == ERROR)
        return ERROR;
<<<<<<< Updated upstream
    log_trace("client, x y %d %d\n", client->pos.x, client->pos.y);
=======
>>>>>>> Stashed changes
    return OK;
}
