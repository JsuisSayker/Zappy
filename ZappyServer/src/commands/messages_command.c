/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** messages_command
*/

#include "zappy_server.h"

void print_messages(zappy_server_t *zappy_server, user_t *user1, user_t *user2)
{
    message_t *message = NULL;
    char *timestamp = NULL;

    dprintf(zappy_server->actual_sockfd, "200|/messages%s", END_LINE);
    for (message = zappy_server->private_messages.tqh_first; message != NULL;
        message = message->next.tqe_next) {
        if ((strcmp(message->receiver_uuid, user1->uuid) == 0 &&
                strcmp(message->sender_uuid, user2->uuid) == 0) ||
            (strcmp(message->sender_uuid, user1->uuid) == 0 ||
                strcmp(message->receiver_uuid, user2->uuid) == 0)) {
            timestamp = ctime(&message->timestamp);
            timestamp[strlen(timestamp) - 1] = '\0';
            dprintf(zappy_server->actual_sockfd, "%s%s%s%s%s%s",
                message->sender_uuid, SPLIT_LINE, timestamp, SPLIT_LINE,
                message->text, END_LINE);
        }
    }
    dprintf(zappy_server->actual_sockfd, END_STR);
}

void messages_command(
    zappy_server_t *zappy_server, char __attribute__((unused)) * command)
{
    user_t *user1 = zappy_server->clients[zappy_server->actual_sockfd].user;
    user_t *user2 = NULL;

    if (user1 == NULL || strlen(command) == 0) {
        dprintf(zappy_server->actual_sockfd, "502|Unauthorized action%s%s",
            END_LINE, END_STR);
        return;
    }
    command = &command[2];
    command[strlen(command) - 1] = '\0';
    user2 = get_user_by_uuid(&zappy_server->all_user, command);
    if (user2 == NULL) {
        dprintf(zappy_server->actual_sockfd, "501|User not found%s%s",
            END_LINE, END_STR);
        return;
    }
    print_messages(zappy_server, user1, user2);
}
