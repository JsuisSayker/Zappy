/*
** EPITECH PROJECT, 2024
** My_teams
** File description:
** create
*/

#include "server.h"

static int create_new_reply(server_data_t *server, client_server_t *client)
{
    thread_t *thread = client->context.thread;
    message_t *new_message = calloc(sizeof(message_t), 1);

    if (new_message == NULL)
        return ERROR;
    if (is_subscribed(client->user, client->context.team) == ERROR) {
        write(client->socket,
            "400, You are not subscribed to this team\a\n", 43);
        free(new_message);
        return ERROR;
    }
    strcpy(new_message->message, client->command->params->comment_body);
    strcpy(new_message->sender_uuid, client->user->uuid);
    get_time(&(new_message)->timestamp);
    TAILQ_INSERT_HEAD(&thread->messages, new_message, entries);
    return create_new_reply_response(server, client, new_message);
}

static int create_new_thread_sub(thread_t *new_thread,
    server_data_t *server, client_server_t *client, channel_t *channel)
{
    char *uuid = generate_uuid();

    strcpy(new_thread->thread_uuid, uuid);
    strcpy(new_thread->title, client->command->params->thread_title);
    strcpy(new_thread->message, client->command->params->thread_body);
    strcpy(new_thread->sender_uuid, client->user->uuid);
    get_time(&(new_thread)->timestamp);
    TAILQ_INIT(&new_thread->messages);
    TAILQ_INSERT_HEAD(&channel->threads, new_thread, entries);
    free(uuid);
    return create_new_thread_response(server, client, new_thread);
}

static int create_new_thread(server_data_t *server, client_server_t *client)
{
    channel_t *channel = client->context.channel;
    thread_t *new_thread = calloc(sizeof(thread_t), 1);

    if (new_thread == NULL)
        return ERROR;
    if (is_subscribed(client->user, client->context.team) == ERROR) {
        write(client->socket,
            "401|/create|You are not subscribed to this team\a\n", 50);
        return OK;
    }
    return create_new_thread_sub(new_thread, server, client, channel);
}

static int create_new_channel_sub(channel_t *new_channel,
    server_data_t *server, client_server_t *client, team_t *team)
{
    char *uuid = generate_uuid();

    strcpy(new_channel->channel_uuid, uuid);
    strcpy(new_channel->channel_name, client->command->params->channel_name);
    strcpy(new_channel->channel_description,
        client->command->params->channel_description);
    TAILQ_INIT(&new_channel->threads);
    TAILQ_INSERT_HEAD(&team->channels, new_channel, entries);
    free(uuid);
    return create_new_channel_response(server, client, new_channel);
}

static int create_new_channel(server_data_t *server, client_server_t *client)
{
    team_t *team = client->context.team;
    channel_t *new_channel = calloc(sizeof(channel_t), 1);

    if (new_channel == NULL)
        return ERROR;
    if (is_subscribed(client->user, client->context.team) == ERROR) {
        write(client->socket,
            "401|/create|You are not subscribed to this team\a\n", 50);
        return OK;
    }
    return create_new_channel_sub(new_channel, server, client, team);
}

static int team_already_exists(team_t *team, char *team_name)
{
    team_t *tmp = team;

    if (tmp == NULL)
        return ERROR;
    while (tmp) {
        if (strcmp(tmp->team_name, team_name) == 0)
            return OK;
        tmp = tmp->entries.tqe_next;
    }
    return ERROR;
}

static int create_new_team(server_data_t *server, client_server_t *client)
{
    char *uuid = generate_uuid();
    team_t *new_team = calloc(sizeof(team_t), 1);

    if (new_team == NULL)
        return ERROR;
    if (team_already_exists(server->teams.tqh_first,
        client->command->params->team_name) == OK) {
        write(client->socket, "403|/create|Team already exists\a\n", 34);
        free(uuid);
        return OK;
    }
    strcpy(new_team->team_uuid, uuid);
    strcpy(new_team->team_name, client->command->params->team_name);
    strcpy(new_team->team_description,
        client->command->params->team_description);
    TAILQ_INIT(&new_team->channels);
    TAILQ_INIT(&new_team->users);
    TAILQ_INSERT_HEAD(&server->teams, new_team, entries);
    free(uuid);
    return create_new_team_response(server, client, new_team);
}

int create(server_data_t *server, client_server_t *client)
{
    if (client->is_logged == false) {
        write(client->socket, "401|/create|You are not logged in\a\n", 36);
        return OK;
    }
    if (client->context.team != NULL && is_subscribed(client->user,
        client->context.team) == ERROR) {
        write(client->socket,
            "401|/create|You are not subscribed to this team\a\n", 50);
        return OK;
    }
    if (client->context.thread != NULL)
        return create_new_reply(server, client);
    if (client->context.channel != NULL)
        return create_new_thread(server, client);
    if (client->context.team != NULL)
        return create_new_channel(server, client);
    if (client->context.team == NULL)
        return create_new_team(server, client);
    return OK;
}
