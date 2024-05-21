/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** logout_command
*/

#include "zappy_server.h"

static int check_command(teams_server_t *teams_server, char *command)
{
    if (strlen(command) != 0) {
        dprintf(teams_server->actual_sockfd, "500|Invalid command%s%s",
            END_LINE, END_STR);
        return KO;
    }
    if (teams_server->clients[teams_server->actual_sockfd].user == NULL) {
        dprintf(teams_server->actual_sockfd, "502|Unauthorized action%s%s",
            END_LINE, END_STR);
        return KO;
    }
    return OK;
}

void logout_command(teams_server_t *teams_server, char *command)
{
    if (check_command(teams_server, command) == KO) {
        return;
    }
    dprintf(teams_server->actual_sockfd, "200|/logout%s%s%s%s%s%s", END_LINE,
        teams_server->clients[teams_server->actual_sockfd].user->uuid,
        SPLIT_LINE,
        teams_server->clients[teams_server->actual_sockfd].user->username,
        END_LINE, END_STR);
    teams_server->clients[teams_server->actual_sockfd].user->nb_clients -= 1;
    teams_server->clients[teams_server->actual_sockfd].user = NULL;
    close(teams_server->actual_sockfd);
    FD_CLR(teams_server->actual_sockfd, &teams_server->fd.save_input);
}
