/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** get_user_by_uuid
*/

#include "zappy_server.h"

user_t *get_user_by_uuid(struct userhead *user_head, char *uuid)
{
    user_t *user = NULL;

    TAILQ_FOREACH(user, user_head, next)
    {
        if (strcmp(user->uuid, uuid) == 0) {
            return user;
        }
    }
    return NULL;
}
