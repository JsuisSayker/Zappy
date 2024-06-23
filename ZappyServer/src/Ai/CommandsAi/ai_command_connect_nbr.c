/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ai_command_right
*/

#include <zappy_server.h>

int ai_command_connect_nbr(zappy_server_t *zappy, client_t *client,
    char *cmd)
{
    team_t *team = NULL;

    if (client == NULL || zappy == NULL || cmd == NULL)
        return ERROR;
    TAILQ_FOREACH(team, &zappy->all_teams, next) {
        if (strcmp(team->name, client->team_name) == 0) {
            dprintf(zappy->actual_sockfd, "%d\n", team->nb_matures_eggs);
            return OK;
        }
    }
    return OK;
}
