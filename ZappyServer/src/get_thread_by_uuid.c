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
