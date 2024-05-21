/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** create_command
*/

#include "zappy_server.h"

static int write_new_reply(int client_fd, reply_t *new_reply, char *team_uuid)
{
    char *timestamp = ctime(&new_reply->timestamp);

    timestamp[strlen(timestamp) - 1] = '\0';
    dprintf(client_fd, "200|/create%sreply%s%s%s%s%s%s%s%s%s%s%s%s", END_LINE,
        END_LINE, team_uuid, SPLIT_LINE, new_reply->thread_uuid, SPLIT_LINE,
        new_reply->sender_uuid, SPLIT_LINE, timestamp, SPLIT_LINE,
        new_reply->text, END_LINE, END_STR);
    return OK;
}

int add_reply(zappy_server_t *zappy_server, char **command_line, int nb_args,
    all_context_t *all_context)
{
    reply_t *new_reply = NULL;

    if (2 != nb_args) {
        dprintf(zappy_server->actual_sockfd, "500|no thread%s%s", END_LINE,
            END_STR);
        return KO;
    }
    new_reply = calloc(sizeof(message_t), 1);
    strcpy(new_reply->text, command_line[1]);
    strcpy(new_reply->sender_uuid,
        zappy_server->clients[zappy_server->actual_sockfd].user->uuid);
    strcpy(new_reply->thread_uuid, all_context->thread->thread_uuid);
    new_reply->timestamp = time(NULL);
    generate_random_uuid(new_reply->reply_uuid);
    TAILQ_INSERT_TAIL(&(all_context->thread->replys_head), new_reply, next);
    write_new_reply(
        zappy_server->actual_sockfd, new_reply, all_context->team->team_uuid);
    return OK;
}

int add_all(zappy_server_t *zappy_server, char **command_line, int nb_args,
    all_context_t *create)
{
    if (add_team(zappy_server, command_line, nb_args, create) == KO) {
        return KO;
    }
    if (add_channel(zappy_server, command_line, nb_args, create) == KO) {
        return KO;
    }
    if (add_thread(zappy_server, command_line, nb_args, create) == KO) {
        return KO;
    }
    if (add_reply(zappy_server, command_line, nb_args, create) == KO) {
        return KO;
    }
    return OK;
}

static int handle_error(zappy_server_t *zappy_server, char *command)
{
    if (zappy_server->clients[zappy_server->actual_sockfd].user == NULL) {
        dprintf(zappy_server->actual_sockfd, "502|Unauthorized action%s%s",
            END_LINE, END_STR);
        return KO;
    }
    if (command[0] != ' ' ||
        (count_str_char(command, '\"') != 2 &&
            count_str_char(command, '\"') != 4)) {
        dprintf(zappy_server->actual_sockfd, "500|Invalid command");
        dprintf(zappy_server->actual_sockfd, "%s%s", END_LINE, END_STR);
        return KO;
    }
    return OK;
}

void create_command(zappy_server_t *zappy_server, char *command)
{
    all_context_t create = {.team = NULL, .channel = NULL, .thread = NULL};
    char **command_line = splitter(command, "\"");
    int nb_args = get_len_char_tab(command_line);

    if (handle_error(zappy_server, command) == KO) {
        free_array(command_line);
        return;
    }
    if (find_all_context(zappy_server, &create.team, &create.channel,
            &create.thread) == KO) {
        free_array(command_line);
        return;
    }
    add_all(zappy_server, command_line, nb_args, &create);
    free_array(command_line);
}
