/*
** EPITECH PROJECT, 2024
** My_teams
** File description:
** free
*/

#include "server.h"

void free_client(client_server_t *client)
{
    if (client) {
        if (client->socket)
            close(client->socket);
        if (client->user_input)
            free(client->user_input);
        if (client->command)
            free_user_input(client->command);
        free(client);
    }
}

void free_clients(server_data_t *server_data)
{
    client_server_t *client = NULL;
    client_server_t *tmp = NULL;

    if (!server_data)
        return;
    while (!TAILQ_EMPTY(&server_data->clients)) {
        client = TAILQ_FIRST(&server_data->clients);
        TAILQ_REMOVE(&server_data->clients, client, entries);
        free_client(client);
    }
}

static void free_user(user_t *user)
{
    if (user) {
        free(user);
    }
}

void free_personnal_messages(user_t *user)
{
    personnal_message_t *message = NULL;

    if (!user)
        return;
    while (!TAILQ_EMPTY(&user->personnal_messages)) {
        message = TAILQ_FIRST(&user->personnal_messages);
        TAILQ_REMOVE(&user->personnal_messages, message, entries);
        if (message)
            free(message);
    }
}

void free_users(server_data_t *server_data)
{
    user_t *user = NULL;
    user_t *tmp = NULL;

    if (!server_data)
        return;
    while (!TAILQ_EMPTY(&server_data->users)) {
        user = TAILQ_FIRST(&server_data->users);
        free_personnal_messages(user);
        TAILQ_REMOVE(&server_data->users, user, entries);
        free(user);
    }
}

static void free_message(message_t *message)
{
    if (message->message)
        free(message->message);
}

static void free_thread(thread_t *thread)
{
    message_t *messages = thread->messages.tqh_first;

    while (messages) {
        TAILQ_REMOVE(&thread->messages, messages, entries);
        free_message(messages);
        messages = thread->messages.tqh_first;
    }
    free(thread);
}

static void free_channel(channel_t *channel)
{
    thread_t *threads = channel->threads.tqh_first;

    while (threads) {
        TAILQ_REMOVE(&channel->threads, threads, entries);
        free_thread(threads);
        threads = channel->threads.tqh_first;
    }
    if (channel->channel_uuid)
        free(channel->channel_uuid);
}

static void free_team(team_t *team)
{
    user_t *users = team->users.tqh_first;
    channel_t *channels = team->channels.tqh_first;

    while (users) {
        TAILQ_REMOVE(&team->users, users, entries);
        free(users);
        users = team->users.tqh_first;
    }
    while (channels) {
        TAILQ_REMOVE(&team->channels, channels, entries);
        free_channel(channels);
        channels = team->channels.tqh_first;
    }
    if (team->team_uuid)
        free(team->team_uuid);
}

void free_teams(server_data_t *server_data)
{
    team_t *team = server_data->teams.tqh_first;

    while (team) {
        TAILQ_REMOVE(&server_data->teams, team, entries);
        free_team(team);
        team = server_data->teams.tqh_first;
    }
}
