/*
** EPITECH PROJECT, 2024
** server_functions
** File description:
** server_functions
*/

#include "zappy_server.h"



static int handle_command(zappy_server_t *zappy_server, char *command)
{
    if (zappy_server->actual_sockfd == STDIN_FILENO)
        return handle_server_command(zappy_server, command);
    switch (zappy_server->clients[zappy_server->actual_sockfd].type) {
    case UNKNOWN:
        return handle_unknown_command(zappy_server, command);
    case IA:
        break;
    case GUI:
        break;
    } 
    return ERROR;
}

static void last_split(
    zappy_server_t *zappy_server, char *buffer, char *last_split)
{
    if (buffer[strlen(buffer) - 1] == *END_LINE) {
        handle_command(zappy_server, last_split);
        memset(buffer, 0, MAX_COMMAND_LENGTH);
    } else {
        strcpy(buffer, last_split);
    }
}

int clean_string(char *buffer)
{
    for (int i = strlen(buffer); i > 0; i -= 1) {
        if (buffer[i] != *END_LINE)
            buffer[i] = '\0';
        else
            return OK;
    }
    return ERROR;
}

void handle_client(zappy_server_t *zappy_server)
{
    int j = 0;
    char buffer[BUFSIZ];
    ssize_t n = read(zappy_server->actual_sockfd, buffer, sizeof(buffer) - 1);
    char **lines = NULL;

    if (n == -1 || n == 0)
        return;
    strcat(zappy_server->clients[zappy_server->actual_sockfd].buffer.
        input_buffer, buffer);
    clean_string(buffer);
    lines = splitter(buffer, END_LINE);
    if (lines == NULL)
        return;
    for (int j = 0; lines[1] != NULL && lines[j + 1]; j += 1) {
        handle_command(zappy_server, lines[j]);
    }
    last_split(zappy_server, buffer, lines[j]);
    free_array(lines);
}
