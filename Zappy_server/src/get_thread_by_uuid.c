/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** get_thread_by_uuid
*/

#include "zappy_server.h"

thread_t *get_thread_by_uuid(struct threadhead *thread_head, char *uuid)
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

thread_t *get_all_thread_by_uuid(struct teamhead *team_head, char *uuid)
{
    team_t *team = NULL;
    channel_t *channel = NULL;
    thread_t *thread = NULL;

    TAILQ_FOREACH(team, team_head, next)
    {
        TAILQ_FOREACH(channel, &team->channels_head, next)
        {
            thread = get_thread_by_uuid(&channel->threads_head, uuid);
            if (thread != NULL) {
                return thread;
            }
        }
    }
    return NULL;
}
