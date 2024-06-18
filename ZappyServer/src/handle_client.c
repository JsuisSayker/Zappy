/*
** EPITECH PROJECT, 2024
** server_functions
** File description:
** server_functions
*/

#include "zappy_server.h"



static int handle_command(zappy_server_t *zappy, char *command)
{
    if (zappy->actual_sockfd == STDIN_FILENO)
        return handle_server_command(zappy, command);
    switch (zappy->clients[zappy->actual_sockfd].type) {
    case UNKNOWN:
        return handle_unknown_command(zappy, command);
    case IA:
        return ai_function(zappy,
            &zappy->clients[zappy->actual_sockfd], command);
    case GUI:
        return handle_gui_command(zappy, command);
    }
    return ERROR;
}

static void last_split(
    zappy_server_t *zappy, char *buffer, char *last_split)
{
    if (buffer[strlen(buffer) - 1] == *END_LINE) {
        handle_command(zappy, last_split);
        memset(buffer, 0, MAX_COMMAND_LENGTH);
    } else {
        strcpy(buffer, last_split);
    }
}

static int clean_string(char *buffer)
{
    for (int i = strlen(buffer); i > 0; i -= 1) {
        if (buffer[i] != *END_LINE)
            buffer[i] = '\0';
        else
            return OK;
    }
    return ERROR;
}

void handle_client(zappy_server_t *zappy)
{
    int j = 0;
    char buffer[BUFSIZ] = {0};
    char **lines = NULL;
    ssize_t n = read(zappy->actual_sockfd, buffer, sizeof(buffer) - 1);

    if (n == -1 || n == 0)
        return;
    strcat(zappy->clients[zappy->actual_sockfd].buffer.
        input_buffer, buffer);
    clean_string(buffer);
    printf("Received: %s\n--\n", buffer);
    lines = splitter(buffer, END_LINE);
    if (lines == NULL)
        return;
    for (j = 0; lines[j] != NULL; j += 1) {
        handle_command(zappy, lines[j]);
    }
    last_split(zappy, buffer, lines[j - 1]);
    free_array(lines);
}
