/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** save_info_to_file
*/

#include "zappy_server.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

static void save_users(struct userhead *user_head, int file, char *str)
{
    user_t *new_user = NULL;

    TAILQ_FOREACH(new_user, user_head, next)
    {
        if (new_user->username[0] != 0) {
            str[0] = USERS_CHAR;
            write(file, str, sizeof(USERS_CHAR));
            write(file, new_user, sizeof(user_t));
        }
    }
}

static void save_privates_messages(
    struct messagehead *message_head, int file, char *str)
{
    message_t *new_message = NULL;

    TAILQ_FOREACH(new_message, message_head, next)
    {
        if (new_message->text[0] != 0) {
            str[0] = PRIVATE_MESSAGE_CHAR;
            write(file, str, sizeof(PRIVATE_MESSAGE_CHAR));
            write(file, new_message, sizeof(message_t));
        }
    }
}

static void save_subscribed(
    struct subscribedhead *subscribe_head, int file, char *str)
{
    subscribed_t *new_subscribe = NULL;

    TAILQ_FOREACH(new_subscribe, subscribe_head, next)
    {
        if (new_subscribe->user_uuid[0] != 0) {
            str[0] = SUBSCRIBE_CHAR;
            write(file, str, sizeof(SUBSCRIBE_CHAR));
            write(file, new_subscribe, sizeof(subscribed_t));
        }
    }
}

static void save_reply(struct replyhead *reply_head, int file, char *str)
{
    reply_t *new_reply = NULL;

    TAILQ_FOREACH(new_reply, reply_head, next)
    {
        if (new_reply->reply_uuid[0] != 0) {
            str[0] = REPLY_CHAR;
            write(file, str, sizeof(REPLY_CHAR));
            write(file, new_reply, sizeof(reply_t));
        }
    }
}

static void save_thread(struct threadhead *thread_head, int file, char *str)
{
    thread_t *new_thread = NULL;

    TAILQ_FOREACH(new_thread, thread_head, next)
    {
        if (new_thread->thread_uuid[0] != 0) {
            str[0] = THREADS_CHAR;
            write(file, str, sizeof(THREADS_CHAR));
            write(file, new_thread, sizeof(thread_t));
            save_reply(&new_thread->replys_head, file, str);
        }
    }
}

static void save_channel(struct channelhead *channel_head, int file, char *str)
{
    channel_t *new_channel = NULL;

    TAILQ_FOREACH(new_channel, channel_head, next)
    {
        if (new_channel->channel_uuid[0] != 0) {
            str[0] = CHANNELS_CHAR;
            write(file, str, sizeof(CHANNELS_CHAR));
            write(file, new_channel, sizeof(channel_t));
            save_thread(&new_channel->threads_head, file, str);
        }
    }
}

static void save_team(struct teamhead *teams_head, int file, char *str)
{
    team_t *new_team = NULL;

    TAILQ_FOREACH(new_team, teams_head, next)
    {
        if (new_team->team_uuid[0] != 0) {
            str[0] = TEAMS_CHAR;
            write(file, str, sizeof(TEAMS_CHAR));
            write(file, new_team, sizeof(team_t));
            save_channel(&new_team->channels_head, file, str);
        }
    }
}

int save_info_to_file(zappy_server_t *zappy_server)
{
    int file = open(SAVE_FILE, O_CREAT | O_TRUNC | O_WRONLY, 00777);
    char str[BUFSIZ];

    memset(str, 0, BUFSIZ);
    if (file == -1) {
        return ERROR;
    }
    save_users(&zappy_server->all_user, file, str);
    save_privates_messages(&zappy_server->private_messages, file, str);
    save_subscribed(&zappy_server->subscribed_teams_users, file, str);
    save_team(&zappy_server->all_teams, file, str);
    close(file);
    return OK;
}
