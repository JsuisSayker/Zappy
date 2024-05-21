/*
** EPITECH PROJECT, 2024
** My_teams
** File description:
** save_data
*/

#include "server.h"

static void save_personnal_messages(user_t *user, int file)
{
    personnal_message_t *tmp = user->personnal_messages.tqh_first;

    while (tmp) {
        write(file, "pers", 4);
        write(file, tmp, sizeof(personnal_message_t));
        tmp = tmp->entries.tqe_next;
    }
}

static void save_users(server_data_t *server_data, int file)
{
    user_t *tmp = server_data->users.tqh_first;

    while (tmp) {
        write(file, "user", 4);
        write(file, tmp, sizeof(user_t));
        save_personnal_messages(tmp, file);
        tmp = tmp->entries.tqe_next;
    }
}

static void save_messages(message_t *message, int file)
{
    message_t *tmp = message;

    while (tmp) {
        write(file, "msgs", 4);
        write(file, tmp, sizeof(message_t));
        tmp = tmp->entries.tqe_next;
    }
}

static void save_threads(thread_t *thread, int file)
{
    thread_t *tmp = thread;

    while (tmp) {
        write(file, "thrd", 4);
        write(file, tmp, sizeof(thread_t));
        save_messages(tmp->messages.tqh_first, file);
        tmp = tmp->entries.tqe_next;
    }
}

static void save_channels(channel_t *channel, int file)
{
    channel_t *tmp = channel;

    while (tmp) {
        write(file, "chan", 4);
        write(file, tmp, sizeof(channel_t));
        save_threads(tmp->threads.tqh_first, file);
        tmp = tmp->entries.tqe_next;
    }
}

static void save_users_subscribed(user_t *user, int file)
{
    user_t *tmp = user;

    while (tmp) {
        write(file, "usub", 4);
        write(file, tmp, sizeof(user_t));
        tmp = tmp->entries.tqe_next;
    }
}

static void save_teams(server_data_t *server_data, int file)
{
    team_t *tmp = server_data->teams.tqh_first;

    while (tmp) {
        write(file, "team", 4);
        write(file, tmp, sizeof(team_t));
        save_channels(tmp->channels.tqh_first, file);
        save_users_subscribed(tmp->users.tqh_first, file);
        tmp = tmp->entries.tqe_next;
    }
}

void save_data(server_data_t *server_data)
{
    int file = open("saves/data.txt", O_CREAT | O_TRUNC | O_WRONLY | O_APPEND,
        00777);

    if (file == -1)
        return;
    save_users(server_data, file);
    save_teams(server_data, file);
    close(file);
}
