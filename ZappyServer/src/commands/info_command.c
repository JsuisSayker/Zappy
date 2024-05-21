/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** info_command
*/
#include "zappy_server.h"

static int info_user(teams_server_t *teams_server, all_context_t *all_context)
{
    if (all_context->team == NULL) {
        dprintf(teams_server->actual_sockfd, "200|/info%suser%s", END_LINE,
            END_LINE);
        if (teams_server->clients[teams_server->actual_sockfd]
                .user->nb_clients > 0) {
            dprintf(teams_server->actual_sockfd, "1%s", SPLIT_LINE);
        } else {
            dprintf(teams_server->actual_sockfd, "0%s", SPLIT_LINE);
        }
        dprintf(teams_server->actual_sockfd, "%s%s%s%s%s",
            teams_server->clients[teams_server->actual_sockfd].user->uuid,
            SPLIT_LINE,
            teams_server->clients[teams_server->actual_sockfd].user->username,
            END_LINE, END_STR);
        return KO;
    }
    return OK;
}

static int info_team(teams_server_t *teams_server, all_context_t *all_context)
{
    if (all_context->channel == NULL) {
        dprintf(teams_server->actual_sockfd, "200|/info%steam%s%s%s%s%s%s%s%s",
            END_LINE, END_LINE, all_context->team->team_uuid, SPLIT_LINE,
            all_context->team->name, SPLIT_LINE, all_context->team->desc,
            END_LINE, END_STR);
        return KO;
    }
    return OK;
}

static int info_channel(
    teams_server_t *teams_server, all_context_t *all_context)
{
    if (all_context->thread == NULL) {
        dprintf(teams_server->actual_sockfd,
            "200|/info%schannel%s%s%s%s%s%s%s%s", END_LINE, END_LINE,
            all_context->channel->channel_uuid, SPLIT_LINE,
            all_context->channel->name, SPLIT_LINE, all_context->channel->desc,
            END_LINE, END_STR);
        return KO;
    }
    return OK;
}

static int info_thread(
    teams_server_t *teams_server, all_context_t *all_context)
{
    char *timestamp = ctime(&all_context->thread->timestamp);

    timestamp[strlen(timestamp) - 1] = '\0';
    dprintf(teams_server->actual_sockfd,
        "200|/info%sthread%s%s%s%s%s%s%s%s%s%s%s%s", END_LINE, END_LINE,
        all_context->thread->thread_uuid, SPLIT_LINE,
        all_context->thread->sender_uuid, SPLIT_LINE, timestamp, SPLIT_LINE,
        all_context->thread->title, SPLIT_LINE, all_context->thread->body,
        END_LINE, END_STR);
    return OK;
}

int info_all(teams_server_t *teams_server, all_context_t *all_context)
{
    if (info_user(teams_server, all_context) == KO) {
        return KO;
    }
    if (info_team(teams_server, all_context) == KO) {
        return KO;
    }
    if (info_channel(teams_server, all_context) == KO) {
        return KO;
    }
    if (info_thread(teams_server, all_context) == KO) {
        return KO;
    }
    return OK;
}

void info_command(teams_server_t *teams_server, char *command)
{
    all_context_t all_context = {
        .team = NULL, .channel = NULL, .thread = NULL};

    if (teams_server->clients[teams_server->actual_sockfd].user == NULL) {
        dprintf(teams_server->actual_sockfd, "502|Unauthorized action%s%s",
            END_LINE, END_STR);
        return;
    }
    if (command[0] != '\0') {
        dprintf(teams_server->actual_sockfd, "500|Invalid command%s%s",
            END_LINE, END_STR);
        return;
    }
    if (find_all_context(teams_server, &all_context.team, &all_context.channel,
            &all_context.thread) == KO) {
        return;
    }
    info_all(teams_server, &all_context);
}
