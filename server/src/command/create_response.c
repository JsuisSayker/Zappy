/*
** EPITECH PROJECT, 2024
** My_teams
** File description:
** create_response
*/

#include "server.h"

int create_new_reply_response(server_data_t *server,
    client_server_t *client, message_t *new_message)
{
    char *response = NULL;

    append_to_string(&response, "200|/create|reply|");
    append_to_string(&response, client->context.thread->thread_uuid);
    append_to_string(&response, "|");
    append_to_string(&response, client->user->uuid);
    append_to_string(&response, "|");
    append_to_string(&response, new_message->timestamp);
    append_to_string(&response, "|");
    append_to_string(&response, client->command->params->comment_body);
    append_to_string(&response, "|");
    append_to_string(&response, client->context.team->team_uuid);
    append_to_string(&response, "\a\n");
    write(client->socket, response, strlen(response));
    free(response);
    server_event_reply_created(client->context.thread->thread_uuid,
        client->user->uuid, client->command->params->comment_body);
    return OK;
}

int create_new_thread_response(server_data_t *server,
    client_server_t *client, thread_t *new_thread)
{
    char *response = NULL;

    append_to_string(&response, "200|/create|thread|");
    append_to_string(&response, new_thread->thread_uuid);
    append_to_string(&response, "|");
    append_to_string(&response, client->user->uuid);
    append_to_string(&response, "|");
    append_to_string(&response, new_thread->timestamp);
    append_to_string(&response, "|");
    append_to_string(&response, new_thread->title);
    append_to_string(&response, "|");
    append_to_string(&response, new_thread->message);
    append_to_string(&response, "\a\n");
    write(client->socket, response, strlen(response));
    free(response);
    server_event_thread_created(client->context.channel->channel_uuid,
        new_thread->thread_uuid, client->user->uuid,
        new_thread->title, new_thread->message);
    return OK;
}

int create_new_channel_response(server_data_t *server,
    client_server_t *client, channel_t *new_channel)
{
    char *response = NULL;

    append_to_string(&response, "200|/create|channel|");
    append_to_string(&response, new_channel->channel_uuid);
    append_to_string(&response, "|");
    append_to_string(&response, new_channel->channel_name);
    append_to_string(&response, "|");
    append_to_string(&response, new_channel->channel_description);
    append_to_string(&response, "\a\n");
    write(client->socket, response, strlen(response));
    free(response);
    server_event_channel_created(client->context.team->team_uuid,
        new_channel->channel_uuid, new_channel->channel_name);
    return OK;
}

int create_new_team_response(server_data_t *server,
    client_server_t *client, team_t *new_team)
{
    char *response = NULL;

    append_to_string(&response, "200|/create|team|");
    append_to_string(&response, new_team->team_uuid);
    append_to_string(&response, "|");
    append_to_string(&response, new_team->team_name);
    append_to_string(&response, "|");
    append_to_string(&response, new_team->team_description);
    append_to_string(&response, "\a\n");
    write(client->socket, response, strlen(response));
    free(response);
    server_event_team_created(new_team->team_uuid, new_team->team_name,
        client->user->uuid);
    return OK;
}
