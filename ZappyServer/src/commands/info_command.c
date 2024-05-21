/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** info_command
*/
#include "zappy_server.h"

static int info_user(zappy_server_t *zappy_server, all_context_t *all_context)
{
    if (all_context->team == NULL) {
        dprintf(zappy_server->actual_sockfd, "200|/info%suser%s", END_LINE,
            END_LINE);
        if (zappy_server->clients[zappy_server->actual_sockfd]
                .user->nb_clients > 0) {
            dprintf(zappy_server->actual_sockfd, "1%s", SPLIT_LINE);
        } else {
            dprintf(zappy_server->actual_sockfd, "0%s", SPLIT_LINE);
        }
        dprintf(zappy_server->actual_sockfd, "%s%s%s%s%s",
            zappy_server->clients[zappy_server->actual_sockfd].user->uuid,
            SPLIT_LINE,
            zappy_server->clients[zappy_server->actual_sockfd].user->username,
            END_LINE, END_STR);
        return KO;
    }
    return OK;
}

static int info_team(zappy_server_t *zappy_server, all_context_t *all_context)
{
    if (all_context->channel == NULL) {
        dprintf(zappy_server->actual_sockfd, "200|/info%steam%s%s%s%s%s%s%s%s",
            END_LINE, END_LINE, all_context->team->team_uuid, SPLIT_LINE,
            all_context->team->name, SPLIT_LINE, all_context->team->desc,
            END_LINE, END_STR);
        return KO;
    }
    return OK;
}

static int info_channel(
    zappy_server_t *zappy_server, all_context_t *all_context)
{
    if (all_context->thread == NULL) {
        dprintf(zappy_server->actual_sockfd,
            "200|/info%schannel%s%s%s%s%s%s%s%s", END_LINE, END_LINE,
            all_context->channel->channel_uuid, SPLIT_LINE,
            all_context->channel->name, SPLIT_LINE, all_context->channel->desc,
            END_LINE, END_STR);
        return KO;
    }
    return OK;
}

static int info_thread(
    zappy_server_t *zappy_server, all_context_t *all_context)
{
    char *timestamp = ctime(&all_context->thread->timestamp);

    timestamp[strlen(timestamp) - 1] = '\0';
    dprintf(zappy_server->actual_sockfd,
        "200|/info%sthread%s%s%s%s%s%s%s%s%s%s%s%s", END_LINE, END_LINE,
        all_context->thread->thread_uuid, SPLIT_LINE,
        all_context->thread->sender_uuid, SPLIT_LINE, timestamp, SPLIT_LINE,
        all_context->thread->title, SPLIT_LINE, all_context->thread->body,
        END_LINE, END_STR);
    return OK;
}

int info_all(zappy_server_t *zappy_server, all_context_t *all_context)
{
    if (info_user(zappy_server, all_context) == KO) {
        return KO;
    }
    if (info_team(zappy_server, all_context) == KO) {
        return KO;
    }
    if (info_channel(zappy_server, all_context) == KO) {
        return KO;
    }
    if (info_thread(zappy_server, all_context) == KO) {
        return KO;
    }
    return OK;
}

void info_command(zappy_server_t *zappy_server, char *command)
{
    all_context_t all_context = {
        .team = NULL, .channel = NULL, .thread = NULL};

    if (zappy_server->clients[zappy_server->actual_sockfd].user == NULL) {
        dprintf(zappy_server->actual_sockfd, "502|Unauthorized action%s%s",
            END_LINE, END_STR);
        return;
    }
    if (command[0] != '\0') {
        dprintf(zappy_server->actual_sockfd, "500|Invalid command%s%s",
            END_LINE, END_STR);
        return;
    }
    if (find_all_context(zappy_server, &all_context.team, &all_context.channel,
            &all_context.thread) == KO) {
        return;
    }
    info_all(zappy_server, &all_context);
}
