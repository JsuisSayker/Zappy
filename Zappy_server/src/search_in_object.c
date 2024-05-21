/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** get_object_by_uuid
*/

#include "../include/zappy_server.h"

thread_t *search_in_threads(struct threadhead *thread_head, char *uuid)
{
    thread_t *thread = NULL;

    TAILQ_FOREACH(thread, thread_head, next)
    {
        if (strcmp(thread->thread_uuid, uuid) == 0) {
            return thread;
        }
    }
    return NULL;
}

channel_t *search_in_channels(struct channelhead *channel_head, char *uuid)
{
    channel_t *channel = NULL;

    TAILQ_FOREACH(channel, channel_head, next)
    {
        if (strcmp(channel->channel_uuid, uuid) == 0) {
            return channel;
        }
    }
    return NULL;
}

team_t *search_in_teams(struct teamhead *team_head, char *uuid)
{
    team_t *team = NULL;

    TAILQ_FOREACH(team, team_head, next)
    {
        if (strcmp(team->team_uuid, uuid) == 0) {
            return team;
        }
    }
    return NULL;
}
