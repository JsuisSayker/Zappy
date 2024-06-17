/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** generate_egg_by_team
*/

#include <zappy_server.h>

void generate_egg_by_team(zappy_server_t *zappy, team_t *new_team, int x,
    int y)
{
    egg_t *new_egg = calloc(sizeof(egg_t), 1);

    if (new_egg == NULL)
        return;
    new_egg->egg_number = zappy->index_eggs;
    new_egg->client_number = -1;
    new_egg->x = x;
    new_egg->y = y;
    TAILQ_INSERT_TAIL(&(new_team->eggs_head), new_egg, next);
    zappy->index_eggs += 1;
}
