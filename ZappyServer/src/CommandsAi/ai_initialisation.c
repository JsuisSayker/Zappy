/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ai_initialisation
*/

#include <zappy_server.h>

static int init_value(
    client_t *ia, team_t *tmp_team, egg_t *new_egg, zappy_server_t *zappy)
{
    struct timeval tv;

    if (ia == NULL || tmp_team == NULL || new_egg == NULL)
        return ERROR;
    gettimeofday(&tv, NULL);
    ia->health.time_to_eat = 126.0 / (double)zappy->args->freq;
    ia->health.last_meal = tv.tv_sec + tv.tv_usec / 1000000.0;
    ia->pos.x = new_egg->x;
    ia->pos.y = new_egg->y;
    ia->team_name = strdup(tmp_team->name);
    ia->client_number = new_egg->client_number;
    tmp_team->nb_drones += 1;
    ia->type = AI;
    ia->level = 1;
    ia->command.execution = NULL;
    return OK;
}

static int init_inventories(client_t *ia)
{
    if (ia == NULL)
        return ERROR;
    ia->inventory.food = 0;
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

bool is_egg_not_use_3(client_t *client, egg_t *tmp_egg, egg_t **tmp_egg_save)
{
    if (client->client_number == tmp_egg->client_number) {
        return false;
    } else {
        if (!(*tmp_egg_save))
            (*tmp_egg_save) = tmp_egg;
    }
    return true;
}

bool is_egg_not_use_2(client_t *client, egg_t *tmp_egg, egg_t **tmp_egg_save)
{
    if (client->type != GUI) {
        if (is_egg_not_use_3(client, tmp_egg, tmp_egg_save) == false)
            return false;
    }
    return true;
}

egg_t *egg_to_not_use(zappy_server_t *zappy, egg_t *tmp_egg)
{
    egg_t *tmp_egg_save = NULL;

    for (int i = 4; i < FD_SETSIZE; i += 1) {
        if (is_egg_not_use_2(&zappy->clients[i], tmp_egg, &tmp_egg_save)
            == false)
            return NULL;
    }
    return tmp_egg_save;
}

egg_t *free_slot_egg(zappy_server_t *zappy, team_t *tmp_team)
{
    egg_t *tmp_egg = NULL;
    egg_t *tmp_egg_save = NULL;

    if (tmp_team == NULL)
        return NULL;
    TAILQ_FOREACH(tmp_egg, &tmp_team->eggs_head, next) {
        tmp_egg_save = egg_to_not_use(zappy, tmp_egg);
        if (tmp_egg_save != NULL)
            return tmp_egg_save;
    }
    return NULL;
}

int ai_initialisation(zappy_server_t *zappy, client_t *ia, team_t *tmp_team)
{
    egg_t *new_egg = NULL;

    if (zappy == NULL || ia == NULL || tmp_team == NULL)
        return ERROR;
    new_egg = free_slot_egg(zappy, tmp_team);
    if (new_egg == NULL)
        return ERROR;
    if (init_value(ia, tmp_team, new_egg, zappy) == ERROR)
        return ERROR;
    TAILQ_REMOVE(&tmp_team->eggs_head, new_egg, next);
    free(new_egg);
    if (init_inventories(ia) != OK)
        return ERROR;
    if (init_queue(ia) != OK)
        return ERROR;
    if (ia->command.queue == NULL)
        return ERROR;
    ia->pos.direction = rand() % 4;
    return OK;
}
