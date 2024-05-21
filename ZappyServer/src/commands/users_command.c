/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** users_command
*/

#include "zappy_server.h"

int check_errors(zappy_server_t *zappy_server, char *command)
{
    if (!command) {
        return 1;
    }
    if (zappy_server->clients[zappy_server->actual_sockfd].user == NULL) {
        dprintf(zappy_server->actual_sockfd, "502|Unauthorized action%s%s",
            END_LINE, END_STR);
        return 1;
    }
    if (strlen(command) != 0) {
        dprintf(zappy_server->actual_sockfd, "500|Internal Server Error%s%s",
            END_LINE, END_STR);
        return 1;
    }
    return 0;
}

void users_command(zappy_server_t *zappy_server, char *command)
{
    user_t *user = NULL;

    if (check_errors(zappy_server, command) == 1) {
        return;
    }
    dprintf(zappy_server->actual_sockfd, "200|/users%s", END_LINE);
    TAILQ_FOREACH(user, &zappy_server->all_user, next)
    {
        if (user->nb_clients > 0) {
            dprintf(zappy_server->actual_sockfd, "1%s", SPLIT_LINE);
        } else {
            dprintf(zappy_server->actual_sockfd, "0%s", SPLIT_LINE);
        }
        dprintf(zappy_server->actual_sockfd, "%s%s%s%s", user->uuid,
            SPLIT_LINE, user->username, END_LINE);
    }
    dprintf(zappy_server->actual_sockfd, END_STR);
}
