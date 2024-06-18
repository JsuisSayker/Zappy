/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ai_initialisation
*/

#include <zappy_server.h>

static int init_value(client_t *ia, team_t *tmp_team, egg_t *new_egg,
    int freq)
{
    struct timeval tv;

    if (ia == NULL || tmp_team == NULL || new_egg == NULL)
        return ERROR;
    gettimeofday(&tv, NULL);
    ia->health.time_to_eat = 126.0 / (double)freq;
    ia->health.last_meal = tv.tv_sec + tv.tv_usec / 1000000.0;
    ia->pos.x = new_egg->x;
    ia->pos.y = new_egg->y;
    ia->team_name = strdup(tmp_team->name);
    ia->client_number = tmp_team->nb_drones;
    tmp_team->nb_drones += 1;
    new_egg->client_number = ia->client_number;
    ia->type = IA;
    ia->level = 1;
    ia->command.execution = NULL;
    return OK;
}

static int init_inventaire(client_t *ia)
{
    if (ia == NULL)
        return ERROR;
    ia->inventory.food = 10;
    ia->inventory.linemate = 0;
    ia->inventory.deraumere = 0;
    ia->inventory.sibur = 0;
    ia->inventory.mendiane = 0;
    ia->inventory.phiras = 0;
    ia->inventory.thystame = 0;
    return OK;
}

static int init_queue(client_t *client)
{
    if (client == NULL)
        return ERROR;
    client->command.queue = calloc(sizeof(char *), 11);
    if (client->command.queue == NULL)
        return ERROR;
    client->command.queue[10] = NULL;
    for (int i = 0; i < 10; i += 1) {
        client->command.queue[i] = NULL;
    }
    return OK;
}

int ai_initialisation(zappy_server_t *zappy, client_t *ia,
    team_t *tmp_team)
{
    egg_t *new_egg = NULL;

    if (zappy == NULL || ia == NULL || tmp_team == NULL)
        return ERROR;
    new_egg = TAILQ_FIRST(&tmp_team->eggs_head);
    for (int i = 0; i < tmp_team->nb_drones; i += 1) {
        new_egg = TAILQ_NEXT(new_egg, next);
    }
    if (init_value(ia, tmp_team, new_egg, zappy->args->freq) == ERROR)
        return ERROR;
    if (init_inventaire(ia) != OK)
        return ERROR;
    if (init_queue(ia) != OK)
        return ERROR;
    if (ia->command.queue == NULL)
        return ERROR;
    ia->pos.direction = rand() % 4;
    return OK;
}
