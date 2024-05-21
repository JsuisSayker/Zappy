/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** create_command
*/

#include "zappy_server.h"

static int write_new_thread(
    int client_fd, thread_t *new_thread, char *user_uuid)
{
    char *timestamp = ctime(&new_thread->timestamp);

    timestamp[strlen(timestamp) - 1] = '\0';
    dprintf(client_fd, "200|/create%sthread%s%s%s%s%s%s%s%s%s%s%s%s", END_LINE,
        END_LINE, new_thread->thread_uuid, SPLIT_LINE, user_uuid, SPLIT_LINE,
        timestamp, SPLIT_LINE, new_thread->title, SPLIT_LINE, new_thread->body,
        END_LINE, END_STR);
    return OK;
}

static int find_thread(struct threadhead *all_thread, char *title)
{
    thread_t *thread = NULL;

    TAILQ_FOREACH(thread, all_thread, next)
    {
        if (strcmp(thread->title, title) == 0) {
            return OK;
        }
    }
    return KO;
}

static int create_thead(teams_server_t *teams_server, char **command_line,
    all_context_t *all_context)
{
    thread_t *new_thread = NULL;

    new_thread = calloc(sizeof(thread_t), 1);
    TAILQ_INIT(&(new_thread->replys_head));
    strcpy(new_thread->title, command_line[1]);
    strcpy(new_thread->body, command_line[3]);
    strcpy(new_thread->channel_uuid, all_context->channel->channel_uuid);
    new_thread->timestamp = time(NULL);
    strcpy(new_thread->sender_uuid,
        teams_server->clients[teams_server->actual_sockfd].user->uuid);
    generate_random_uuid(new_thread->thread_uuid);
    TAILQ_INSERT_TAIL(&(all_context->channel->threads_head), new_thread, next);
    write_new_thread(teams_server->actual_sockfd, new_thread,
        teams_server->clients[teams_server->actual_sockfd].user->uuid);
    return OK;
}

int add_thread(teams_server_t *teams_server, char **command_line, int nb_args,
    all_context_t *all_context)
{
    if (all_context->thread == NULL) {
        if (4 != nb_args) {
            dprintf(teams_server->actual_sockfd, "500|no thread%s%s", END_LINE,
                END_STR);
            return KO;
        }
        if (find_thread(&(all_context->channel->threads_head),
                command_line[1]) == OK) {
            dprintf(teams_server->actual_sockfd, "503|/create%s%s", END_LINE,
                END_STR);
            return KO;
        }
        create_thead(teams_server, command_line, all_context);
        return KO;
    }
    return OK;
}
