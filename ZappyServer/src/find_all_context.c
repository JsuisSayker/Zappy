/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** find_all
*/

#include "zappy_server.h"

static int find_team(zappy_server_t *zappy_server, team_t **team)
{
    if (zappy_server->clients[zappy_server->actual_sockfd]
            .user->team_context[0] != '\0') {
        *team = search_in_teams(&zappy_server->all_teams,
            zappy_server->clients[zappy_server->actual_sockfd]
                .user->team_context);
        if (*team == NULL) {
            dprintf(zappy_server->actual_sockfd, "504| Unknow team%s%s%s%s",
                END_STR,
                zappy_server->clients[zappy_server->actual_sockfd]
                    .user->team_context,
                END_LINE, END_STR);
            return KO;
        }
    }
    return OK;
}

