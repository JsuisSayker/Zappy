/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** logout_command
*/

#include "zappy_server.h"

static int check_command(zappy_server_t *zappy_server, char *command)
{
    if (strlen(command) != 0) {
        dprintf(zappy_server->actual_sockfd, "500|Invalid command%s%s",
            END_LINE, END_STR);
        return KO;
    }
    if (zappy_server->clients[zappy_server->actual_sockfd].user == NULL) {
        dprintf(zappy_server->actual_sockfd, "502|Unauthorized action%s%s",
            END_LINE, END_STR);
        return KO;
    }
    return OK;
}

void logout_command(zappy_server_t *zappy_server, char *command)
{
    if (check_command(zappy_server, command) == KO) {
        return;
    }
    dprintf(zappy_server->actual_sockfd, "200|/logout%s%s%s%s%s%s", END_LINE,
        zappy_server->clients[zappy_server->actual_sockfd].user->uuid,
        SPLIT_LINE,
        zappy_server->clients[zappy_server->actual_sockfd].user->username,
        END_LINE, END_STR);
    zappy_server->clients[zappy_server->actual_sockfd].user->nb_clients -= 1;
    zappy_server->clients[zappy_server->actual_sockfd].user = NULL;
    close(zappy_server->actual_sockfd);
    FD_CLR(zappy_server->actual_sockfd, &zappy_server->fd.save_input);
}
