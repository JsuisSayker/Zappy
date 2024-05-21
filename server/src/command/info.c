/*
** EPITECH PROJECT, 2024
** My_teams
** File description:
** info
*/

#include "server.h"

static int info_thread(server_data_t *server_data, client_server_t *client)
{
    char *response = NULL;

    append_to_string(&response, "200|/info|reply|");
    append_to_string(&response, client->context.thread->thread_uuid);
    append_to_string(&response, "|");
    append_to_string(&response, client->context.thread->thread_uuid);
    append_to_string(&response, "|");
    append_to_string(&response, client->context.thread->timestamp);
    append_to_string(&response, "|");
    append_to_string(&response, client->context.thread->message);
    append_to_string(&response, "\a\n");
    write(client->socket, response, strlen(response));
    free(response);
    return OK;
}

static int info_channel(server_data_t *server_data, client_server_t *client)
{
    char *response = NULL;

    append_to_string(&response, "200|/info|channel|");
    append_to_string(&response, client->context.channel->channel_uuid);
    append_to_string(&response, "|");
    append_to_string(&response, client->context.channel->channel_name);
    append_to_string(&response, "|");
    append_to_string(&response, client->context.channel->channel_description);
    append_to_string(&response, "\a\n");
    write(client->socket, response, strlen(response));
    free(response);
    return OK;
}

static int info_team(server_data_t *server_data, client_server_t *client)
{
    char *response = NULL;

    append_to_string(&response, "200|/info|team|");
    append_to_string(&response, client->context.team->team_uuid);
    append_to_string(&response, "|");
    append_to_string(&response, client->context.team->team_name);
    append_to_string(&response, "|");
    append_to_string(&response, client->context.team->team_description);
    append_to_string(&response, "\a\n");
    write(client->socket, response, strlen(response));
    free(response);
    return OK;
}

static int info_user(server_data_t *server_data, client_server_t *client)
{
    char *response = NULL;

    append_to_string(&response, "200|/info|user|");
    append_to_string(&response, client->user->uuid);
    append_to_string(&response, "|");
    append_to_string(&response, client->user->username);
    append_to_string(&response, "|");
    if (client->user->user_connected > 0)
        append_to_string(&response, "1");
    else
        append_to_string(&response, "0");
    append_to_string(&response, "\a\n");
    write(client->socket, response, strlen(response));
    free(response);
    return OK;
}

int info(server_data_t *server_data, client_server_t *client)
{
    if (client->is_logged == false) {
        write(client->socket, "401|/create|You are not logged in\a\n", 36);
        return ERROR;
    }
    if (client->context.thread != NULL)
        return info_thread(server_data, client);
    if (client->context.channel != NULL)
        return info_channel(server_data, client);
    if (client->context.team != NULL)
        return info_team(server_data, client);
    return info_user(server_data, client);
}
