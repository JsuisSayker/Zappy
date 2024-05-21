/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** use_command
*/

#include "zappy_server.h"

int handle_errors(zappy_server_t *zappy_server, char *command)
{
    if (zappy_server->clients[zappy_server->actual_sockfd].user == NULL) {
        dprintf(zappy_server->actual_sockfd, "502|Unauthorized action%s%s",
            END_LINE, END_STR);
        return 1;
    }
    if (count_str_char(command, '\"') != 0 &&
        count_str_char(command, '\"') != 2 &&
        count_str_char(command, '\"') != 4 &&
        count_str_char(command, '\"') != 6) {
        dprintf(zappy_server->actual_sockfd, "500|Internal Server Error\n");
        dprintf(zappy_server->actual_sockfd, END_STR);
        return 1;
    }
    return 0;
}

int get_array_len(char **array)
{
    int i = 0;

    for (; array[i] != NULL; i++)
        ;
    return i;
}

int fill_context_2(zappy_server_t *zappy_server, char **split_command)
{
    if (get_array_len(split_command) == 4) {
        strcpy(zappy_server->clients[zappy_server->actual_sockfd]
                .user->team_context,
            split_command[1]);
        strcpy(zappy_server->clients[zappy_server->actual_sockfd]
                .user->channel_context,
            split_command[3]);
    }
    if (get_array_len(split_command) == 6) {
        strcpy(zappy_server->clients[zappy_server->actual_sockfd]
                .user->team_context,
            split_command[1]);
        strcpy(zappy_server->clients[zappy_server->actual_sockfd]
                .user->channel_context,
            split_command[3]);
        strcpy(zappy_server->clients[zappy_server->actual_sockfd]
                .user->thread_context,
            split_command[5]);
    }
    return 0;
}

int fill_context(zappy_server_t *zappy_server, char *command)
{
    char **split_command = splitter(command, "\"");

    memset(
        zappy_server->clients[zappy_server->actual_sockfd].user->team_context,
        0, MAX_UUID_LENGTH);
    memset(zappy_server->clients[zappy_server->actual_sockfd]
            .user->channel_context, 0, MAX_UUID_LENGTH);
    memset(zappy_server->clients[zappy_server->actual_sockfd]
            .user->thread_context, 0, MAX_UUID_LENGTH);
    if (split_command == NULL) {
        free_array(split_command);
        return 1;
    }
    if (get_array_len(split_command) == 2) {
        strcpy(zappy_server->clients[zappy_server->actual_sockfd]
                .user->team_context, split_command[1]);
    }
    fill_context_2(zappy_server, split_command);
    free_array(split_command);
    return 0;
}

void use_command(zappy_server_t *zappy_server, char *command)
{
    if (handle_errors(zappy_server, command) == 1) {
        return;
    }
    fill_context(zappy_server, command);
    dprintf(zappy_server->actual_sockfd, "200|/use%s", END_LINE);
    dprintf(zappy_server->actual_sockfd, END_STR);
}
