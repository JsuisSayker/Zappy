/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** subscribed_command
*/

#include "zappy_server.h"

int list_subscribed_teams(teams_server_t *teams_server)
{
    team_t *team = NULL;
    subscribed_t *subscribed = NULL;

    dprintf(teams_server->actual_sockfd, "200|/subscribed%steams%s", END_LINE,
        END_LINE);
    TAILQ_FOREACH(subscribed, &teams_server->subscribed_teams_users, next) {
        if (strcmp(subscribed->user_uuid, teams_server->clients
            [teams_server->actual_sockfd].user->uuid) != 0) {
            continue;
        }
        team =
            get_team_by_uuid(&teams_server->all_teams, subscribed->team_uuid);
        if (team) {
            dprintf(teams_server->actual_sockfd, "%s%s%s%s%s%s",
                team->team_uuid, SPLIT_LINE, team->name, SPLIT_LINE,
                team->desc, END_LINE);
        }
    }
    dprintf(teams_server->actual_sockfd, END_STR);
    return 0;
}

void write_user_status(int client_fd, user_t *user)
{
    if (user->nb_clients > 0) {
        dprintf(client_fd, "1%s", SPLIT_LINE);
    } else {
        dprintf(client_fd, "0%s", SPLIT_LINE);
    }
}

int list_subscribed_users(teams_server_t *teams_server, char *team_uuid)
{
    user_t *user = NULL;
    subscribed_t *subscribed = NULL;

    dprintf(teams_server->actual_sockfd, "200|/subscribed%susers%s", END_LINE,
        END_LINE);
    TAILQ_FOREACH(subscribed, &teams_server->subscribed_teams_users, next) {
        if (strcmp(subscribed->team_uuid, team_uuid) != 0) {
            continue;
        }
        user =
            get_user_by_uuid(&teams_server->all_user, subscribed->user_uuid);
        if (user) {
            write_user_status(teams_server->actual_sockfd, user);
            dprintf(teams_server->actual_sockfd, "%s%s%s%s", user->uuid,
                SPLIT_LINE, user->username, END_LINE);
        }
    }
    dprintf(teams_server->actual_sockfd, END_STR);
    return 0;
}

void subscribed_command(
    teams_server_t *teams_server, char __attribute__((unused)) * command)
{
    char **splitted_command = splitter(command, "\"");

    if (teams_server->clients[teams_server->actual_sockfd].user == NULL) {
        dprintf(teams_server->actual_sockfd, "502|Unauthorized action%s%s",
            END_LINE, END_STR);
        free_array(splitted_command);
        return;
    }
    if (splitted_command == NULL) {
        list_subscribed_teams(teams_server);
    } else {
        list_subscribed_users(teams_server, splitted_command[1]);
    }
    free_array(splitted_command);
}
