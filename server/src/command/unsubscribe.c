/*
** EPITECH PROJECT, 2024
** My_teams
** File description:
** unsubscibe
*/

#include "server.h"

static void remove_user_from_team(team_t *team, user_t *user)
{
    user_t *tmp = team->users.tqh_first;

    if (tmp == NULL)
        return;
    while (tmp) {
        if (strcmp(tmp->uuid, user->uuid) == 0) {
            TAILQ_REMOVE(&team->users, tmp, entries);
            free(tmp);
            return;
        }
        tmp = tmp->entries.tqe_next;
    }
}

static int unsubscribe_response(user_t *user, team_t *team,
    client_server_t *client)
{
    char *response = NULL;

    append_to_string(&response, "200|/unsubscribe|");
    append_to_string(&response, user->uuid);
    append_to_string(&response, "|");
    append_to_string(&response, team->team_uuid);
    append_to_string(&response, "\a\n");
    write(client->socket, response, strlen(response));
    free(response);
    server_event_user_unsubscribed(team->team_uuid, user->uuid);
    return OK;
}

int unsubscribe(server_data_t *server_data, client_server_t *client)
{
    user_t *user = client->user;
    team_t *team = get_team_by_uuid(server_data->teams.tqh_first,
        client->command->params->team_uuid);

    if (client->is_logged == false) {
        write(client->socket, "401|/unsubscribe|not logged\a\n", 30);
        return KO;
    }
    if (team == NULL) {
        send_team_not_found(client, "/unsubscribe", "/team");
        return KO;
    }
    if (is_subscribed(user, team) == OK)
        remove_user_from_team(team, user);
    return unsubscribe_response(user, team, client);
}
