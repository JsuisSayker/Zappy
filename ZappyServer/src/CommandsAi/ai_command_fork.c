/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ai_command_right
*/

#include <zappy_server.h>

void add_egg(zappy_server_t *zappy, client_t *client, team_t *team)
{
    egg_t *new_egg = calloc(sizeof(egg_t), 1);

    if (new_egg == NULL)
        return;
    new_egg->egg_number = zappy->index_eggs;
    new_egg->client_number = -1;
    new_egg->x = client->pos.x;
    new_egg->y = client->pos.y;
    TAILQ_INSERT_TAIL(&(team->eggs_head), new_egg, next);
    zappy->index_eggs += 1;
    team->nb_matures_eggs += 1;
    return;
}

static int do_fork(zappy_server_t *zappy, client_t *client)
{
    team_t *team = NULL;
    egg_t *tmp_egg;

    TAILQ_FOREACH(team, &zappy->all_teams, next) {
        if (strcmp(team->name, client->team_name) == 0) {
            add_egg(zappy, client, team);
            tmp_egg = TAILQ_LAST(&(team->eggs_head), egghead);
            send_enw_command_to_all_gui(zappy, tmp_egg);
            send_pfk_command_to_all_gui(zappy, client);
            dprintf(zappy->actual_sockfd, "ok\n");
            return OK;
        }
    }
    return KO;
}

int ai_command_fork(zappy_server_t *zappy, client_t *client, char *cmd)
{
    if (client == NULL || zappy == NULL || cmd == NULL)
        return ERROR;
    if (cast_action(zappy, client, 42, cmd) == ERROR)
        return ERROR;
    if (check_action(zappy, client) == false)
        return OK;
    if (do_fork(zappy, client) == OK)
        return OK;
    return KO;
}
