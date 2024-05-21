/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** create_command
*/

#include "zappy_server.h"

static int write_new_team(int client_fd, team_t *new_team)
{
    dprintf(client_fd, "200|/create%steam%s%s%s%s%s%s%s%s", END_LINE, END_LINE,
        new_team->team_uuid, SPLIT_LINE, new_team->name, SPLIT_LINE,
        new_team->desc, END_LINE, END_STR);
    return OK;
}

static int find_team(struct teamhead *all_teams, char *name)
{
    team_t *team = NULL;

    TAILQ_FOREACH(team, all_teams, next)
    {
        if (strcmp(team->name, name) == 0) {
            return OK;
        }
    }
    return KO;
}

static int create_team(teams_server_t *teams_server, char **command_line)
{
    team_t *new_team = NULL;

    new_team = calloc(sizeof(team_t), 1);
    TAILQ_INIT(&(new_team->channels_head));
    strcpy(new_team->name, command_line[1]);
    strcpy(new_team->desc, command_line[3]);
    generate_random_uuid(new_team->team_uuid);
    TAILQ_INSERT_TAIL(&(teams_server->all_teams), new_team, next);
    write_new_team(teams_server->actual_sockfd, new_team);
    return OK;
}

int add_team(teams_server_t *teams_server, char **command_line, int nb_args,
    all_context_t *all_context)
{
    if (all_context->team == NULL) {
        if (4 != nb_args) {
            dprintf(teams_server->actual_sockfd, "500|Server Error%s%s",
                END_LINE, END_STR);
            return KO;
        }
        if (find_team(&(teams_server->all_teams), command_line[1]) == OK) {
            dprintf(teams_server->actual_sockfd, "503|/create%s%s", END_LINE,
                END_STR);
            return KO;
        }
        create_team(teams_server, command_line);
        return KO;
    }
    return OK;
}
