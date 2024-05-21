/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** subscribe_command
*/

#include "zappy_server.h"

bool is_team_exist(zappy_server_t *zappy_server, char *team_uuid)
{
    team_t *team = NULL;

    TAILQ_FOREACH(team, &zappy_server->all_teams, next)
    {
        if (strcmp(team->team_uuid, team_uuid) == 0) {
            return true;
        }
    }
    return false;
}

static int handle_error(zappy_server_t *zappy_server, char *command)
{
    if (zappy_server->clients[zappy_server->actual_sockfd].user == NULL) {
        dprintf(zappy_server->actual_sockfd, "502|Unauthorized action%s%s",
            END_LINE, END_STR);
        return KO;
    }
    if (strlen(command) < 2) {
        dprintf(zappy_server->actual_sockfd, "500|Internal Server Error\n");
        dprintf(zappy_server->actual_sockfd, END_STR);
        return KO;
    }
    return OK;
}

subscribed_t *add_subscribed_team(
    zappy_server_t *zappy_server, char *team_uuid)
{
    subscribed_t *subscribe = NULL;

    subscribe = calloc(sizeof(subscribed_t), 1);
    strcpy(subscribe->team_uuid, team_uuid);
    strcpy(subscribe->user_uuid,
        zappy_server->clients[zappy_server->actual_sockfd].user->uuid);
    TAILQ_INSERT_TAIL(
        &(zappy_server->subscribed_teams_users), subscribe, next);
    return subscribe;
}

void subscribe_command(zappy_server_t *zappy_server, char *command)
{
    subscribed_t *subscribe = NULL;

    if (handle_error(zappy_server, command) == KO) {
        return;
    }
    command = &command[2];
    command[strlen(command) - 1] = '\0';
    if (!is_team_exist(zappy_server, command)) {
        dprintf(zappy_server->actual_sockfd, "504|Team not found\n");
        dprintf(zappy_server->actual_sockfd, END_STR);
        return;
    }
    subscribe = add_subscribed_team(zappy_server, command);
    dprintf(zappy_server->actual_sockfd, "200|/subscribe%s%s%s%s%s%s",
        END_LINE,
        zappy_server->clients[zappy_server->actual_sockfd].user->uuid,
        SPLIT_LINE, subscribe->team_uuid, END_LINE, END_STR);
}
