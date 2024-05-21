/*
** EPITECH PROJECT, 2024
** My_teams
** File description:
** subscribe
*/

#include "server.h"

static int subscribe_response(team_t *team, user_t *user,
    client_server_t *client)
{
    char *response = NULL;

    append_to_string(&response, "200|/subscribe|");
    append_to_string(&response, user->uuid);
    append_to_string(&response, "|");
    append_to_string(&response, team->team_uuid);
    append_to_string(&response, "\a\n");
    write(client->socket, response, strlen(response));
    free(response);
    server_event_user_subscribed(team->team_uuid, user->uuid);
    return OK;
}

int is_subscribed(user_t *user, team_t *team)
{
    user_t *tmp;

    if (user == NULL || team == NULL)
        return ERROR;
    tmp = team->users.tqh_first;
    while (tmp) {
        if (strcmp(tmp->uuid, user->uuid) == 0)
            return OK;
        tmp = tmp->entries.tqe_next;
    }
    return ERROR;
}

static user_t *copy_user(user_t *user)
{
    user_t *new_user = calloc(sizeof(user_t), 1);

    if (new_user == NULL)
        return NULL;
    strcpy(new_user->uuid, user->uuid);
    strcpy(new_user->username, user->username);
    return new_user;
}

int subscribe(server_data_t *server, client_server_t *client)
{
    team_t *team = NULL;
    user_t *user = NULL;

    if (client->is_logged == false) {
        write(client->socket, "401|/subscribe|not logged\a\n", 28);
        return OK;
    }
    team = get_team_by_uuid(server->teams.tqh_first,
    client->command->params->team_uuid);
    if (team == NULL) {
        send_team_not_found(client, "/subscribe", "/team");
        return OK;
    }
    if (is_subscribed(user, team) == OK) {
        write(client->socket, "403|/subscribe| already subscribed\a\n", 37);
        return OK;
    }
    user = copy_user(client->user);
    TAILQ_INSERT_HEAD(&team->users, user, entries);
    return subscribe_response(team, user, client);
}
