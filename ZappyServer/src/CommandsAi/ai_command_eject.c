/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ai_command_eject
*/

#include "zappy_server.h"

static ai_position_t direction(client_t *client)
{
    ai_position_t vector = {0, 0, 0};

    switch (client->pos.direction) {
    case NORTH:
        vector.y = -1;
        return vector;
    case SOUTH:
        vector.y = 1;
        return vector;
    case EAST:
        vector.x = 1;
        return vector;
    case WEST:
        vector.x = -1;
        return vector;
    }
    return vector;
}

static int destroy_egg(zappy_server_t *zappy, client_t *client)
{
    team_t *tmp = NULL;
    egg_t *egg = NULL;

    if (client == NULL || zappy == NULL) {
        return ERROR;
    }
    tmp = zappy->all_teams.tqh_first;
    TAILQ_FOREACH(tmp, &zappy->all_teams, next) {
        TAILQ_FOREACH(egg, &tmp->eggs_head, next) {
            if (egg->x == client->pos.x && egg->y == client->pos.y) {
                send_die_command_to_all_gui(zappy, egg->egg_number);
                TAILQ_REMOVE(&tmp->eggs_head, egg, next);
                free(egg);
            }
        }
    }
    return OK;
}

static int eject_player(zappy_server_t *zappy, client_t *client,
    ai_position_t vector)
{
    if (zappy == NULL || client == NULL)
        return ERROR;
    for (int i = 0; i < zappy->args->clientsNb; i++) {
        if (zappy->clients[i].type != AI &&
            client->client_number != zappy->clients[i].client_number &&
            client->pos.x == zappy->clients[i].pos.x && client->pos.y ==
            zappy->clients[i].pos.y) {
            zappy->clients[i].pos.x += vector.x;
            zappy->clients[i].pos.y += vector.y;
            dprintf(i, "eject: 1\n");
            send_ppo_command_to_all_gui(zappy, &zappy->clients[i]);
        }
    }
    destroy_egg(zappy, client);
    send_pex_command_to_all_gui(zappy, client->client_number);
    dprintf(zappy->actual_sockfd, "ok\n");
    return OK;
}

int ai_command_eject(zappy_server_t *zappy, client_t *client, char *cmd)
{
    ai_position_t vector;

    if (client == NULL || zappy == NULL || cmd == NULL)
        return ERROR;
    if (cast_action(zappy, client, 7, cmd) == ERROR)
        return ERROR;
    if (check_action(zappy, client) == false)
        return OK;
    vector = direction(client);
    if (eject_player(zappy, client, vector) == ERROR){
        dprintf(zappy->actual_sockfd, "ko\n");
        return ERROR;
    }
    return OK;
}
