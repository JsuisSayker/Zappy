/*
** EPITECH PROJECT, 2024
** MyTeams
** File description:
** get_func
*/

#include <time.h>

#include "server.h"

team_t *get_team_by_uuid(team_t *teams, char *uuid)
{
    team_t *tmp = teams;

    if (teams == NULL || uuid == NULL)
        return NULL;
    while (tmp) {
        if (strcmp(tmp->team_uuid, uuid) == 0)
            return tmp;
        tmp = tmp->entries.tqe_next;
    }
    return NULL;
}

int get_time(char (*dest)[TIME_LENGTH])
{
    time_t actual_time;
    struct tm *time_info;
    char time_form[TIME_LENGTH];

    if ((*dest) == NULL)
        return ERROR;
    time(&actual_time);
    time_info = localtime(&actual_time);
    strftime(time_form, sizeof(time_form), "%a %b %d %H:%M:%S %Y", time_info);
    if (strcpy((*dest), time_form) == NULL)
        return ERROR;
    return OK;
}

user_t *get_user_by_uuid(server_data_t *server, char *uuid)
{
    user_t *tmp;

    if (server == NULL || uuid == NULL)
        return NULL;
    tmp = server->users.tqh_first;
    TAILQ_FOREACH(tmp, &server->users, entries) {
        if (strcmp(tmp->uuid, uuid) == 0)
            return tmp;
    }
    return NULL;
}
