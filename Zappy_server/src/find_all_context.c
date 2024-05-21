/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** find_all
*/

#include "zappy_server.h"

static int find_team(teams_server_t *teams_server, team_t **team)
{
    if (teams_server->clients[teams_server->actual_sockfd]
            .user->team_context[0] != '\0') {
        *team = search_in_teams(&teams_server->all_teams,
            teams_server->clients[teams_server->actual_sockfd]
                .user->team_context);
        if (*team == NULL) {
            dprintf(teams_server->actual_sockfd, "504| Unknow team%s%s%s%s",
                END_STR,
                teams_server->clients[teams_server->actual_sockfd]
                    .user->team_context,
                END_LINE, END_STR);
            return KO;
        }
    }
    return OK;
}

static int find_channel(
    teams_server_t *teams_server, team_t *team, channel_t **channel)
{
    if (teams_server->clients[teams_server->actual_sockfd]
            .user->channel_context[0] != '\0') {
        *channel = search_in_channels(&team->channels_head,
            teams_server->clients[teams_server->actual_sockfd]
                .user->channel_context);
        if (*channel == NULL) {
            dprintf(teams_server->actual_sockfd, "505|Unknown channel%s%s%s%s",
                END_STR,
                teams_server->clients[teams_server->actual_sockfd]
                    .user->channel_context,
                END_LINE, END_STR);
            return KO;
        }
    }
    return OK;
}

static int find_thread(
    teams_server_t *teams_server, channel_t *channel, thread_t **thread)
{
    if (teams_server->clients[teams_server->actual_sockfd]
            .user->thread_context[0] != '\0') {
        *thread = search_in_threads(&channel->threads_head,
            teams_server->clients[teams_server->actual_sockfd]
                .user->thread_context);
        if (*thread == NULL) {
            dprintf(teams_server->actual_sockfd, "506|Unknown thread%s%s%s%s",
                END_STR,
                teams_server->clients[teams_server->actual_sockfd]
                    .user->thread_context,
                END_LINE, END_STR);
            return KO;
        }
    }
    return OK;
}

int find_all_context(teams_server_t *teams_server, team_t **team,
    channel_t **channel, thread_t **thread)
{
    if (find_team(teams_server, team) == KO) {
        return KO;
    }
    if (find_channel(teams_server, *team, channel) == KO) {
        return KO;
    }
    if (find_thread(teams_server, *channel, thread) == KO) {
        return KO;
    }
    return OK;
}
