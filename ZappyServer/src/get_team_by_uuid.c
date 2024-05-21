/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** get_team_by_uuid
*/

#include "zappy_server.h"

team_t *get_team_by_uuid(struct teamhead *teams_head, char *uuid)
{
    team_t *team = NULL;

    TAILQ_FOREACH(team, teams_head, next)
    {
        if (strcmp(team->team_uuid, uuid) == 0) {
            return team;
        }
    }
    return NULL;
}
