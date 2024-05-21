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
    teams_server_t *teams_server, user_t *receiver_user, char *message_body)
{
    for (int i = 4; i < FD_SETSIZE; i += 1) {
        if (teams_server->clients[i].user &&
            strcmp(teams_server->clients[i].user->uuid, receiver_user->uuid) ==
                0) {
            dprintf(i, "200|/send%s%s%s%s%s%s", END_LINE,
                teams_server->clients[teams_server->actual_sockfd].user->uuid,
                SPLIT_LINE, message_body, END_LINE, END_STR);
            return OK;
        }
    }
    return KO;
}

static int loop_user(teams_server_t *teams_server, char **parsed_command)
{
    user_t *receiver_user =
        get_user_by_uuid(&teams_server->all_user, parsed_command[1]);
    message_t *message = NULL;

    if (receiver_user == NULL) {
        return KO;
    }
    message = create_message(
        teams_server->clients[teams_server->actual_sockfd].user->uuid,
        receiver_user->uuid, parsed_command[3]);
    TAILQ_INSERT_TAIL(&(teams_server->private_messages), message, next);
    send_message_receiver(teams_server, receiver_user, parsed_command[3]);
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

static int handle_error(teams_server_t *teams_server, char **parsed_command)
{
    if (teams_server->clients[teams_server->actual_sockfd].user == NULL) {
        dprintf(teams_server->actual_sockfd, "502|Unauthorized action%s%s",
            END_LINE, END_STR);
        return KO;
    }
    if (!parsed_command) {
        dprintf(teams_server->actual_sockfd, "500|Internal Server Error\n");
        dprintf(teams_server->actual_sockfd, END_STR);
        return KO;
    }
    return OK;
}

void send_command(teams_server_t *teams_server, char *command)
{
    char **parsed_command = parse_command(command);

    if (handle_error(teams_server, parsed_command) == KO) {
        free_array(parsed_command);
        return;
    }
    if (loop_user(teams_server, parsed_command) == KO) {
        dprintf(teams_server->actual_sockfd, "500|user not found\n");
        dprintf(teams_server->actual_sockfd, END_STR);
    }
    free_array(parsed_command);
}
