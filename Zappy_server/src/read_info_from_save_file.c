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

static int add_save_user(teams_server_t *teams_server, int file)
{
    user_t *user1 = calloc(sizeof(user_t), 1);

    if (read(file, user1, sizeof(user_t)) == -1) {
        return KO;
    }
    if (user1->username[0] == '\0' || user1->uuid[0] == '\0') {
        free(user1);
    } else {
        memset(user1->team_context, 0, MAX_UUID_LENGTH);
        memset(user1->channel_context, 0, MAX_UUID_LENGTH);
        memset(user1->thread_context, 0, MAX_UUID_LENGTH);
        user1->nb_clients = 0;
        TAILQ_INSERT_TAIL(&teams_server->all_user, user1, next);
    }
    return OK;
}

static int add_save_private_message(teams_server_t *teams_server, int file)
{
    message_t *private_message = calloc(sizeof(message_t), 1);

    if (read(file, private_message, sizeof(message_t)) == -1) {
        return KO;
    }
    if (private_message->sender_uuid[0] == '\0' ||
        private_message->receiver_uuid[0] == '\0') {
        free(private_message);
        return OK;
    }
    TAILQ_INSERT_TAIL(&teams_server->private_messages, private_message, next);
    return OK;
}

static int add_save_subscribe(teams_server_t *teams_server, int file)
{
    subscribed_t *subscribe = calloc(sizeof(subscribed_t), 1);

    if (read(file, subscribe, sizeof(subscribed_t)) == -1) {
        return KO;
    }
    if (subscribe->user_uuid[0] == '\0' || subscribe->team_uuid[0] == '\0') {
        free(subscribe);
        return OK;
    }
    TAILQ_INSERT_TAIL(&teams_server->subscribed_teams_users, subscribe, next);
    return OK;
}

static int add_save_team(teams_server_t *teams_server, int file)
{
    team_t *new_team = calloc(sizeof(team_t), 1);

    if (read(file, new_team, sizeof(team_t)) == -1) {
        return KO;
    }
    if (new_team->team_uuid[0] == '\0') {
        free(new_team);
        return OK;
    }
    new_team->next.tqe_next = NULL;
    new_team->next.tqe_prev = NULL;
    TAILQ_INIT(&new_team->channels_head);
    TAILQ_INSERT_TAIL(&teams_server->all_teams, new_team, next);
    return OK;
}

static int add_save_channel(teams_server_t *teams_server, int file)
{
    team_t *team = NULL;
    channel_t *new_channel = calloc(sizeof(channel_t), 1);

    if (read(file, new_channel, sizeof(channel_t)) == -1) {
        return KO;
    }
    if (new_channel->channel_uuid[0] == '\0') {
        free(new_channel);
        return OK;
    }
    new_channel->next.tqe_next = NULL;
    new_channel->next.tqe_prev = NULL;
    TAILQ_INIT(&new_channel->threads_head);
    team = get_team_by_uuid(&teams_server->all_teams, new_channel->team_uuid);
    if (team != NULL) {
        TAILQ_INSERT_TAIL(&team->channels_head, new_channel, next);
    }
    return OK;
}

static int add_save_thread(teams_server_t *teams_server, int file)
{
    channel_t *channel = NULL;
    thread_t *new_thread = calloc(sizeof(thread_t), 1);

    if (read(file, new_thread, sizeof(thread_t)) == -1) {
        return KO;
    }
    if (new_thread->thread_uuid[0] == '\0') {
        free(new_thread);
        return OK;
    }
    new_thread->next.tqe_next = NULL;
    new_thread->next.tqe_prev = NULL;
    TAILQ_INIT(&new_thread->replys_head);
    channel = get_all_channel_by_uuid(
        &teams_server->all_teams, new_thread->channel_uuid);
    if (channel != NULL) {
        TAILQ_INSERT_TAIL(&channel->threads_head, new_thread, next);
        return OK;
    }
    return OK;
}

static int add_save_reply(teams_server_t *teams_server, int file)
{
    thread_t *thread = NULL;
    reply_t *new_reply = calloc(sizeof(reply_t), 1);

    if (read(file, new_reply, sizeof(reply_t)) == -1) {
        return KO;
    }
    if (new_reply->reply_uuid[0] == '\0') {
        free(new_reply);
        return OK;
    }
    new_reply->next.tqe_next = NULL;
    new_reply->next.tqe_prev = NULL;
    thread = get_all_thread_by_uuid(
        &teams_server->all_teams, new_reply->thread_uuid);
    if (thread != NULL) {
        TAILQ_INSERT_TAIL(&thread->replys_head, new_reply, next);
        return OK;
    }
    return OK;
}

int choose_elem_2(teams_server_t *teams_server, int file, char delimiter)
{
    switch (delimiter) {
    case CHANNELS_CHAR:
        add_save_channel(teams_server, file);
        break;
    case THREADS_CHAR:
        add_save_thread(teams_server, file);
        break;
    case REPLY_CHAR:
        add_save_reply(teams_server, file);
        break;
    default:
        break;
    }
    return OK;
}

int choose_elem(teams_server_t *teams_server, int file, char delimiter)
{
    switch (delimiter) {
    case USERS_CHAR:
        add_save_user(teams_server, file);
        break;
    case PRIVATE_MESSAGE_CHAR:
        add_save_private_message(teams_server, file);
        break;
    case SUBSCRIBE_CHAR:
        add_save_subscribe(teams_server, file);
        break;
    case TEAMS_CHAR:
        add_save_team(teams_server, file);
        break;
    default:
        choose_elem_2(teams_server, file, delimiter);
        break;
    }
    return OK;
}

int read_info_from_save_file(teams_server_t *teams_server)
{
    int file = open(SAVE_FILE, O_RDONLY, 00777);
    int n_byte = 0;
    char str[BUFSIZ];

    memset(str, 0, BUFSIZ);
    if (file == -1) {
        return KO;
    }
    do {
        n_byte = read(file, str, sizeof(USERS_CHAR));
        if (n_byte == -1) {
            break;
        }
        choose_elem(teams_server, file, str[0]);
    } while (n_byte != 0);
    close(file);
    return OK;
}
