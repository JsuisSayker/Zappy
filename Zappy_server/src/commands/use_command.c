/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** use_command
*/

#include "zappy_server.h"

int handle_errors(teams_server_t *teams_server, char *command)
{
    if (teams_server->clients[teams_server->actual_sockfd].user == NULL) {
        dprintf(teams_server->actual_sockfd, "502|Unauthorized action%s%s",
            END_LINE, END_STR);
        return 1;
    }
    if (count_str_char(command, '\"') != 0 &&
        count_str_char(command, '\"') != 2 &&
        count_str_char(command, '\"') != 4 &&
        count_str_char(command, '\"') != 6) {
        dprintf(teams_server->actual_sockfd, "500|Internal Server Error\n");
        dprintf(teams_server->actual_sockfd, END_STR);
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

int fill_context_2(teams_server_t *teams_server, char **split_command)
{
    if (get_array_len(split_command) == 4) {
        strcpy(teams_server->clients[teams_server->actual_sockfd]
                .user->team_context,
            split_command[1]);
        strcpy(teams_server->clients[teams_server->actual_sockfd]
                .user->channel_context,
            split_command[3]);
    }
    if (get_array_len(split_command) == 6) {
        strcpy(teams_server->clients[teams_server->actual_sockfd]
                .user->team_context,
            split_command[1]);
        strcpy(teams_server->clients[teams_server->actual_sockfd]
                .user->channel_context,
            split_command[3]);
        strcpy(teams_server->clients[teams_server->actual_sockfd]
                .user->thread_context,
            split_command[5]);
    }
    return 0;
}

int fill_context(teams_server_t *teams_server, char *command)
{
    char **split_command = splitter(command, "\"");

    memset(
        teams_server->clients[teams_server->actual_sockfd].user->team_context,
        0, MAX_UUID_LENGTH);
    memset(teams_server->clients[teams_server->actual_sockfd]
            .user->channel_context, 0, MAX_UUID_LENGTH);
    memset(teams_server->clients[teams_server->actual_sockfd]
            .user->thread_context, 0, MAX_UUID_LENGTH);
    if (split_command == NULL) {
        free_array(split_command);
        return 1;
    }
    if (get_array_len(split_command) == 2) {
        strcpy(teams_server->clients[teams_server->actual_sockfd]
                .user->team_context, split_command[1]);
    }
    fill_context_2(teams_server, split_command);
    free_array(split_command);
    return 0;
}

void use_command(teams_server_t *teams_server, char *command)
{
    if (handle_errors(teams_server, command) == 1) {
        return;
    }
    fill_context(teams_server, command);
    dprintf(teams_server->actual_sockfd, "200|/use%s", END_LINE);
    dprintf(teams_server->actual_sockfd, END_STR);
}
