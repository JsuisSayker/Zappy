/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** list_command
*/
#include "zappy_server.h"

static int list_team(teams_server_t *teams_server, all_context_t *all_context)
{
    team_t *actual_team = NULL;

    if (all_context->team == NULL) {
        dprintf(teams_server->actual_sockfd, "200|/list%steam%s", END_LINE,
            END_LINE);
        TAILQ_FOREACH(actual_team, &(teams_server->all_teams), next)
        {
            dprintf(teams_server->actual_sockfd, "%s%s%s%s%s%s",
                actual_team->team_uuid, SPLIT_LINE, actual_team->name,
                SPLIT_LINE, actual_team->desc, END_LINE);
        }
        dprintf(teams_server->actual_sockfd, END_STR);
        return KO;
    }
    return OK;
}

static int list_channel(
    teams_server_t *teams_server, all_context_t *all_context)
{
    channel_t *actual_channel = NULL;

    if (all_context->channel == NULL) {
        dprintf(teams_server->actual_sockfd, "200|/list%schannel%s", END_LINE,
            END_LINE);
        TAILQ_FOREACH(
            actual_channel, &(all_context->team->channels_head), next)
        {
            dprintf(teams_server->actual_sockfd, "%s%s%s%s%s%s",
                actual_channel->channel_uuid, SPLIT_LINE, actual_channel->name,
                SPLIT_LINE, actual_channel->desc, END_LINE);
        }
        dprintf(teams_server->actual_sockfd, END_STR);
        return KO;
    }
    return OK;
}

static void print_thread_in_fd(
    teams_server_t *teams_server, thread_t *actual_thread)
{
    char *timestamp = NULL;

    timestamp = ctime(&actual_thread->timestamp);
    timestamp[strlen(timestamp) - 1] = '\0';
    dprintf(teams_server->actual_sockfd, "%s%s%s%s%s%s%s%s%s%s",
        actual_thread->thread_uuid, SPLIT_LINE, actual_thread->sender_uuid,
        SPLIT_LINE, timestamp, SPLIT_LINE, actual_thread->title, SPLIT_LINE,
        actual_thread->body, END_LINE);
}

static int list_thread(
    teams_server_t *teams_server, all_context_t *all_context)
{
    thread_t *actual_thread = NULL;

    if (all_context->thread == NULL) {
        dprintf(teams_server->actual_sockfd, "200|/list%sthread%s", END_LINE,
            END_LINE);
        TAILQ_FOREACH(
            actual_thread, &(all_context->channel->threads_head), next)
        {
            print_thread_in_fd(teams_server, actual_thread);
        }
        dprintf(teams_server->actual_sockfd, END_STR);
        return KO;
    }
    return OK;
}

static int list_reply(teams_server_t *teams_server, all_context_t *all_context)
{
    reply_t *actual_reply = NULL;
    char *timestamp = NULL;

    dprintf(
        teams_server->actual_sockfd, "200|/list%sreply%s", END_LINE, END_LINE);
    for (actual_reply = all_context->thread->replys_head.tqh_first;
        actual_reply != NULL; actual_reply = actual_reply->next.tqe_next) {
        timestamp = ctime(&actual_reply->timestamp);
        timestamp[strlen(timestamp) - 1] = '\0';
        dprintf(teams_server->actual_sockfd, "%s%s%s%s%s%s%s%s%s",
            actual_reply->thread_uuid, SPLIT_LINE, actual_reply->sender_uuid,
            SPLIT_LINE, timestamp, SPLIT_LINE, actual_reply->text, SPLIT_LINE,
            END_LINE);
    }
    dprintf(teams_server->actual_sockfd, END_STR);
    return OK;
}

int list_all(teams_server_t *teams_server, all_context_t *all_context)
{
    if (list_team(teams_server, all_context) == KO) {
        return KO;
    }
    if (list_channel(teams_server, all_context) == KO) {
        return KO;
    }
    if (list_thread(teams_server, all_context) == KO) {
        return KO;
    }
    if (list_reply(teams_server, all_context) == KO) {
        return KO;
    }
    return OK;
}

void list_command(teams_server_t *teams_server, char *command)
{
    all_context_t all_context = {
        .team = NULL, .channel = NULL, .thread = NULL};

    if (teams_server->clients[teams_server->actual_sockfd].user == NULL) {
        dprintf(teams_server->actual_sockfd, "502|Unauthorized action%s%s",
            END_LINE, END_STR);
        return;
    }
    if (strlen(command) != 0) {
        dprintf(teams_server->actual_sockfd, "500|Invalid command\n");
        dprintf(teams_server->actual_sockfd, END_STR);
        return;
    }
    if (find_all_context(teams_server, &all_context.team, &all_context.channel,
            &all_context.thread) == KO) {
        return;
    }
    list_all(teams_server, &all_context);
}
