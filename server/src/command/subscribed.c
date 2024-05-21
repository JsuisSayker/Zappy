/*
** EPITECH PROJECT, 2024
** My_teams
** File description:
** subscribed
*/

#include "server.h"

static int subscribed_with_team_response(server_data_t *server,
    client_server_t *client, team_t *team)
{
    char *response = NULL;
    user_t *users = team->users.tqh_first;

    append_to_string(&response, "200|/subscribed|/users|");
    append_to_string(&response, team->team_uuid);
    TAILQ_FOREACH(users, &team->users, entries) {
        append_to_string(&response, "|");
        append_to_string(&response, users->uuid);
        append_to_string(&response, "|");
        append_to_string(&response, users->username);
        append_to_string(&response, "|");
        if (users->user_connected > 0)
            append_to_string(&response, "1");
        else
            append_to_string(&response, "0");
    }
    append_to_string(&response, "\a\n");
    write(client->socket, response, strlen(response));
    free(response);
    return OK;
}

static int subscribed_without_team(server_data_t *server,
    client_server_t *client)
{
    char *response = NULL;
    team_t *teams = server->teams.tqh_first;

    append_to_string(&response, "200|/subscribed|/teams|");
    TAILQ_FOREACH(teams, &server->teams, entries) {
        if (is_subscribed(client->user, teams) == OK) {
            append_to_string(&response, "|");
            append_to_string(&response, teams->team_uuid);
            append_to_string(&response, "|");
            append_to_string(&response, teams->team_name);
            append_to_string(&response, "|");
            append_to_string(&response, teams->team_description);
        }
    }
    append_to_string(&response, "\a\n");
    write(client->socket, response, strlen(response));
    free(response);
    return OK;
}

static int subscribed_with_team(server_data_t *server, client_server_t *client)
{
    team_t *team = get_team_by_uuid(server->teams.tqh_first,
        client->command->params->team_uuid);

    if (team == NULL) {
        write(client->socket, "500|/subscribed|Team not found\a\n", 33);
        return OK;
    }
    if (is_subscribed(client->user, team) == ERROR) {
        write(client->socket,
            "401|/subscribed|You are not subscribed to this team\a\n", 54);
        return OK;
    }
    return subscribed_with_team_response(server, client, team);
}

int subscribed(server_data_t *server, client_server_t *client)
{
    if (client->is_logged == false) {
    write(client->socket, "401|/subscribed|You are not logged in\a\n", 40);
        return ERROR;
    }
    if (client->command->params->team_uuid != NULL)
        return subscribed_with_team(server, client);
    return subscribed_without_team(server, client);
}
