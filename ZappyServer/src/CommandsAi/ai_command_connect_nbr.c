/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ai_command_right
*/

#include <zappy_server.h>

static void unused_slots(zappy_server_t *zappy, client_t *client)
{
    team_t *team = NULL;

    TAILQ_FOREACH(team, &zappy->all_teams, next) {
        if (strcmp(team->name, client->team_name) == 0) {
            dprintf(zappy->actual_sockfd, "%d\n", team->nb_matures_eggs);
            return;
        }
    }
}

int ai_command_connect_nbr(zappy_server_t *zappy, client_t *client,
    char *cmd)
{
    if (client == NULL || zappy == NULL || cmd == NULL)
        return ERROR;
    if (cast_action(zappy, client, 0, cmd) == ERROR)
        return ERROR;
    if (check_action(zappy, client) == false)
        return OK;
    unused_slots(zappy, client);
    return OK;
}
