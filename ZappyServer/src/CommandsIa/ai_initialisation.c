/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ai_initialisation
*/

#include "zappy_server.h"

static int ai_value_direction_setter(client_t *ia)
{
    int rdm_orientation = 0;

    if (ia == NULL)
        return ERROR;
    rdm_orientation = rand() % 3;
    if (rdm_orientation == 0)
        ia->pos.direction = NORTH;
    if (rdm_orientation == 1)
        ia->pos.direction = EAST;
    if (rdm_orientation == 2)
        ia->pos.direction = SOUTH;
    if (rdm_orientation == 3)
        ia->pos.direction = WEST;
    return OK;
}

static int init_value(client_t *ia, team_t *tmp_team, egg_t *new_egg,
    int freq)
{
    struct timeval tv;

    if (ia == NULL || tmp_team == NULL || new_egg == NULL)
        return ERROR;
    gettimeofday(&tv, NULL);
    ia->health.time_to_eat = 126.0 / (double)freq;
    ia->health.last_meal = tv.tv_sec + tv.tv_usec / 1000000.0;
    ia->health.is_alive = true;
    ia->pos.x = new_egg->x;
    ia->pos.y = new_egg->y;
    ia->team_name = strdup(tmp_team->name);
    ia->client_number = tmp_team->nb_drones;
    new_egg->client_number = ia->client_number;
    ia->type = IA;
    ia->level = 1;
    ia->command.execusion = NULL;
    return OK;
}

static int init_queue(client_t *client)
{
    if (client == NULL)
        return ERROR;
    client->command.queue = malloc(sizeof(char *) * 11);
    if (client->command.queue == NULL)
        return ERROR;
    client->command.queue[10] = NULL;
    for (int i = 0; i < 10; i += 1)
        client->command.queue[i] = NULL;
    return OK;
}

int ai_initialisation(zappy_server_t *zappy_server, client_t *ia,
    team_t *tmp_team)
{
    egg_t *new_egg = NULL;

    if (zappy_server == NULL || ia == NULL || tmp_team == NULL)
        return ERROR;
    new_egg = TAILQ_FIRST(&tmp_team->eggs_head);
    for (int i = 0; i < tmp_team->nb_drones; i += 1) {
        new_egg = TAILQ_NEXT(new_egg, next);
    }
    if (init_value(ia, tmp_team, new_egg, zappy_server->args->freq) == ERROR)
        return ERROR;
    if (init_queue(ia) != OK)
        return ERROR;
    if (ia->command.queue == NULL)
        return ERROR;
    if (ai_value_direction_setter(ia) == ERROR)
        return ERROR;
    return OK;
}
