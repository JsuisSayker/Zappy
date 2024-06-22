/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ai_command_right
*/

#include <zappy_server.h>

static int do_fork(zappy_server_t *zappy, client_t *client)
{
    team_t *team = NULL;
    egg_t *tmp_egg;

    TAILQ_FOREACH(team, &zappy->all_teams, next) {
        if (strcmp(team->name, client->team_name) == 0) {
            generate_egg_by_team(zappy, team, client->pos.x, client->pos.y);
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
    printf("    FORK\n");
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
