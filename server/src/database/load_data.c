/*
** EPITECH PROJECT, 2024
** My_teams
** File description:
** load_data
*/

#include "server.h"

static void load_personnal_message(server_data_t *server_data,
    load_data_t *load_data, int file)
{
    personnal_message_t *message = calloc(sizeof(personnal_message_t), 1);

    if (message == NULL)
        return;
    if (read(file, message, sizeof(personnal_message_t)) <= 0) {
        free(message);
        return;
    }
    TAILQ_INSERT_HEAD(&load_data->user->personnal_messages, message, entries);
}

static void load_user(server_data_t *server_data, load_data_t *load_data,
    int file)
{
    user_t *user = calloc(sizeof(user_t), 1);

    if (user == NULL)
        return;
    if (read(file, user, sizeof(user_t)) <= 0) {
        free(user);
        return;
    }
    TAILQ_INIT(&user->personnal_messages);
    TAILQ_INSERT_HEAD(&server_data->users, user, entries);
    server_data->users.tqh_first->user_connected = 0;
    load_data->user = user;
    server_event_user_loaded(user->uuid, user->username);
}

static void load_thread(server_data_t *server_data, load_data_t *load_data,
    int file)
{
    thread_t *thread = calloc(sizeof(thread_t), 1);

    if (thread == NULL)
        return;
    if (read(file, thread, sizeof(thread_t)) <= 0) {
        free(thread);
        return;
    }
    TAILQ_INIT(&thread->messages);
    TAILQ_INSERT_HEAD(&load_data->channel->threads, thread, entries);
    load_data->thread = thread;
}

static void load_channel(server_data_t *server_data, load_data_t *load_data,
    int file)
{
    channel_t *channel = calloc(sizeof(channel_t), 1);

    if (channel == NULL)
        return;
    if (read(file, channel, sizeof(channel_t)) <= 0) {
        free(channel);
        return;
    }
    TAILQ_INIT(&channel->threads);
    TAILQ_INSERT_HEAD(&load_data->team->channels, channel, entries);
    load_data->channel = channel;
}

static void load_team(server_data_t *server_data, load_data_t *load_data,
    int file)
{
    team_t *team = calloc(sizeof(team_t), 1);

    if (team == NULL)
        return;
    if (read(file, team, sizeof(team_t)) <= 0) {
        free(team);
        return;
    }
    TAILQ_INIT(&team->users);
    TAILQ_INIT(&team->channels);
    TAILQ_INSERT_HEAD(&server_data->teams, team, entries);
    load_data->team = team;
}

static void load_message(server_data_t *server_data, load_data_t *load_data,
    int file)
{
    message_t *message = calloc(sizeof(message_t), 1);

    if (message == NULL)
        return;
    if (read(file, message, sizeof(message_t)) <= 0) {
        free(message);
        return;
    }
    TAILQ_INSERT_HEAD(&load_data->thread->messages, message, entries);
}

static void load_users_subscribed(server_data_t *server_data,
    load_data_t *load_data, int file)
{
    user_t *user = calloc(sizeof(user_t), 1);

    if (user == NULL)
        return;
    if (read(file, user, sizeof(user_t)) <= 0) {
        free(user);
        return;
    }
    TAILQ_INSERT_HEAD(&load_data->team->users, user, entries);
}

static void check_load_data(server_data_t *server_data, load_data_t *load_data,
    char *buffer, int file)
{
    if (strcmp(buffer, "user") == 0)
        load_user(server_data, load_data, file);
    if (strcmp(buffer, "team") == 0)
        load_team(server_data, load_data, file);
    if (strcmp(buffer, "chan") == 0)
        load_channel(server_data, load_data, file);
    if (strcmp(buffer, "thrd") == 0)
        load_thread(server_data, load_data, file);
    if (strcmp(buffer, "pers") == 0)
        load_personnal_message(server_data, load_data, file);
    if (strcmp(buffer, "msgs") == 0)
        load_message(server_data, load_data, file);
    if (strcmp(buffer, "usub") == 0)
        load_users_subscribed(server_data, load_data, file);
}

void load_data(server_data_t *server_data)
{
    char buffer[BUFSIZ];
    load_data_t *load_data = calloc(sizeof(load_data_t), 1);
    int file = open("saves/data.txt", O_RDONLY, 00777);

    if (file == -1)
        return;
    while (read(file, buffer, 4) > 0) {
        buffer[4] = '\0';
        check_load_data(server_data, load_data, buffer, file);
    }
    free(load_data);
    close(file);
}
