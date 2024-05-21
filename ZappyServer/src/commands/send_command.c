/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** send_command
*/

#include "zappy_server.h"

message_t *create_message(char *sender_uuid, char *receiver_uuid, char *text)
{
    message_t *message = malloc(sizeof(message_t));

    if (!message) {
        return NULL;
    }
    strcpy(message->sender_uuid, sender_uuid);
    strcpy(message->receiver_uuid, receiver_uuid);
    strcpy(message->text, text);
    message->timestamp = time(NULL);
    generate_random_uuid(message->message_uuid);
    return message;
}

static int send_message_receiver(
    zappy_server_t *zappy_server, user_t *receiver_user, char *message_body)
{
    for (int i = 4; i < FD_SETSIZE; i += 1) {
        if (zappy_server->clients[i].user &&
            strcmp(zappy_server->clients[i].user->uuid, receiver_user->uuid) ==
                0) {
            dprintf(i, "200|/send%s%s%s%s%s%s", END_LINE,
                zappy_server->clients[zappy_server->actual_sockfd].user->uuid,
                SPLIT_LINE, message_body, END_LINE, END_STR);
            return OK;
        }
    }
    return KO;
}

static int loop_user(zappy_server_t *zappy_server, char **parsed_command)
{
    user_t *receiver_user =
        get_user_by_uuid(&zappy_server->all_user, parsed_command[1]);
    message_t *message = NULL;

    if (receiver_user == NULL) {
        return KO;
    }
    message = create_message(
        zappy_server->clients[zappy_server->actual_sockfd].user->uuid,
        receiver_user->uuid, parsed_command[3]);
    TAILQ_INSERT_TAIL(&(zappy_server->private_messages), message, next);
    send_message_receiver(zappy_server, receiver_user, parsed_command[3]);
    return OK;
}

char **parse_command(char *command)
{
    char **parsed_command = splitter(command, "\"");

    if (get_len_char_tab(parsed_command) != 4) {
        return NULL;
    }
    return parsed_command;
}

static int handle_error(zappy_server_t *zappy_server, char **parsed_command)
{
    if (zappy_server->clients[zappy_server->actual_sockfd].user == NULL) {
        dprintf(zappy_server->actual_sockfd, "502|Unauthorized action%s%s",
            END_LINE, END_STR);
        return KO;
    }
    if (!parsed_command) {
        dprintf(zappy_server->actual_sockfd, "500|Internal Server Error\n");
        dprintf(zappy_server->actual_sockfd, END_STR);
        return KO;
    }
    return OK;
}

void send_command(zappy_server_t *zappy_server, char *command)
{
    char **parsed_command = parse_command(command);

    if (handle_error(zappy_server, parsed_command) == KO) {
        free_array(parsed_command);
        return;
    }
    if (loop_user(zappy_server, parsed_command) == KO) {
        dprintf(zappy_server->actual_sockfd, "500|user not found\n");
        dprintf(zappy_server->actual_sockfd, END_STR);
    }
    free_array(parsed_command);
}
