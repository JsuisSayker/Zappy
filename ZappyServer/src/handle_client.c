/*
** EPITECH PROJECT, 2024
** server_functions
** File description:
** server_functions
*/

#include "zappy_server.h"

static const struct command_s COMMAND_FUNCS[] = {{"/help", &help_command},
    {"/login", &login_command}, {"/logout", &logout_command},
    {"/users", &users_command}, {"/user", &user_command},
    {"/send", &send_command}, {"/messages", &messages_command},
    {"/subscribed", &subscribed_command}, {"/subscribe", &subscribe_command},
    {"/unsubscribe", &unsubscribe_command}, {"/use", &use_command},
    {"/create", &create_command}, {"/list", &list_command},
    {"/info", &info_command}, {"NULL", NULL}};

void handle_command(teams_server_t *teams_server, char *command)
{
    for (int i = 0; COMMAND_FUNCS[i].func != NULL; i += 1) {
        if (strncmp(command, COMMAND_FUNCS[i].command,
                strlen(COMMAND_FUNCS[i].command)) == 0) {
            COMMAND_FUNCS[i].func(
                teams_server, &command[strlen(COMMAND_FUNCS[i].command)]);
            return;
        }
    }
}

static void last_split(
    teams_server_t *teams_server, char *buffer, char *last_split)
{
    if (buffer[strlen(buffer) - 1] == *END_STR) {
        handle_command(teams_server, last_split);
        memset(buffer, 0, MAX_COMMAND_LENGTH);
    } else {
        strcpy(buffer, last_split);
    }
}

void handle_client(teams_server_t *teams_server)
{
    int j = 0;
    char buffer[BUFSIZ];
    ssize_t n = read(teams_server->actual_sockfd, buffer, sizeof(buffer) - 1);
    char **lines = NULL;

    if (n == -1 || n == 0) {
        return;
    }
    strcat(
        teams_server->clients[teams_server->actual_sockfd].buffer.input_buffer,
        buffer);
    lines = splitter(buffer, END_STR);
    if (lines == NULL) {
        return;
    }
    for (; lines[1] != NULL && lines[j + 1]; j += 1) {
        handle_command(teams_server, lines[j]);
    }
    last_split(teams_server, buffer, lines[j]);
    free_array(lines);
}
