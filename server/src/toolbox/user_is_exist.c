/*
** EPITECH PROJECT, 2024
** MyTeams
** File description:
** user_is_exit
*/

#include "server.h"

bool user_is_exist(server_data_t *server, char *uuid)
{
    user_t *tmp = server->users.tqh_first;

    if (server == NULL || uuid == NULL)
        return false;
    TAILQ_FOREACH(tmp, &server->users, entries) {
        if (strcmp(tmp->uuid, uuid) == 0)
            return true;
    }
    return false;
}
