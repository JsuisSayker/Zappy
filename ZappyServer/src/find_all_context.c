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

static int find_channel(
    zappy_server_t *zappy_server, team_t *team, channel_t **channel)
{
    if (zappy_server->clients[zappy_server->actual_sockfd]
            .user->channel_context[0] != '\0') {
        *channel = search_in_channels(&team->channels_head,
            zappy_server->clients[zappy_server->actual_sockfd]
                .user->channel_context);
        if (*channel == NULL) {
            dprintf(zappy_server->actual_sockfd, "505|Unknown channel%s%s%s%s",
                END_STR,
                zappy_server->clients[zappy_server->actual_sockfd]
                    .user->channel_context,
                END_LINE, END_STR);
            return KO;
        }
    }
    return OK;
}

static int find_thread(
    zappy_server_t *zappy_server, channel_t *channel, thread_t **thread)
{
    if (zappy_server->clients[zappy_server->actual_sockfd]
            .user->thread_context[0] != '\0') {
        *thread = search_in_threads(&channel->threads_head,
            zappy_server->clients[zappy_server->actual_sockfd]
                .user->thread_context);
        if (*thread == NULL) {
            dprintf(zappy_server->actual_sockfd, "506|Unknown thread%s%s%s%s",
                END_STR,
                zappy_server->clients[zappy_server->actual_sockfd]
                    .user->thread_context,
                END_LINE, END_STR);
            return KO;
        }
    }
    return OK;
}

int find_all_context(zappy_server_t *zappy_server, team_t **team,
    channel_t **channel, thread_t **thread)
{
    if (find_team(zappy_server, team) == KO) {
        return KO;
    }
    if (find_channel(zappy_server, *team, channel) == KO) {
        return KO;
    }
    if (find_thread(zappy_server, *channel, thread) == KO) {
        return KO;
    }
    return OK;
}
