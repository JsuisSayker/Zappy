/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** unsubscribe_command
*/

#include "zappy_server.h"

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

void remove_subscribed(zappy_server_t *zappy_server, subscribed_t *subscribe)
{
    TAILQ_REMOVE(&zappy_server->subscribed_teams_users, subscribe, next);
    dprintf(zappy_server->actual_sockfd, "200|/unsubscribe%s%s%s%s%s%s",
        END_LINE, subscribe->user_uuid, SPLIT_LINE, subscribe->team_uuid,
        END_LINE, END_STR);
    free(subscribe);
}

void unsubscribe_command(
    zappy_server_t *zappy_server, char __attribute__((unused)) * command)
{
    subscribed_t *subscribe = NULL;

    if (handle_error(zappy_server, command) == KO) {
        return;
    }
    command = &command[2];
    command[strlen(command) - 1] = '\0';
    TAILQ_FOREACH(subscribe, &zappy_server->subscribed_teams_users, next)
    {
        if (strcmp(subscribe->team_uuid, command) == 0 &&
            strcmp(subscribe->user_uuid,
                zappy_server->clients[zappy_server->actual_sockfd]
                    .user->uuid) == 0) {
            remove_subscribed(zappy_server, subscribe);
            return;
        }
    }
}
