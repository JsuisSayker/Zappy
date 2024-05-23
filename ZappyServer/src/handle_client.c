/*
** EPITECH PROJECT, 2024
** server_functions
** File description:
** server_functions
*/

#include "zappy_server.h"

static void handle_command(zappy_server_t *zappy_server, char *command)
{
    if (zappy_server->actual_sockfd == 1) {
        return handle_ia_command(zappy_server, command);
    }
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

void handle_client(zappy_server_t *zappy_server)
{
    int j = 0;
    char buffer[BUFSIZ];
    ssize_t n = read(zappy_server->actual_sockfd, buffer, sizeof(buffer) - 1);
    char **lines = NULL;

    if (n == -1 || n == 0) {
        return;
    }
    strcat(
        zappy_server->clients[zappy_server->actual_sockfd].buffer.input_buffer,
        buffer);
    for (int i = strlen(buffer); i > 0; i -= 1) {
        if (buffer[i] != *END_LINE) {
            buffer[i] = '\0';
        } else {
            break;
        }
    }

    lines = splitter(buffer, END_LINE);
    if (lines == NULL) {
        return;
    }
    for (int j = 0; lines[1] != NULL && lines[j + 1]; j += 1) {
        handle_command(zappy_server, lines[j]);
    }
    last_split(zappy_server, buffer, lines[j]);
    free_array(lines);
}
