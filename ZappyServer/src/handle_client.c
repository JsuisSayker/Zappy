/*
** EPITECH PROJECT, 2024
** server_functions
** File description:
** server_functions
*/

#include "zappy_server.h"

static const struct path_type_s COMMAND_FUNCS[] = {
    {"Forward", &type_ai},
    {"Right", &type_ai},
    {"Left", &type_ai},
    {"Take", &type_ai},
    {"Set", &type_ai},
    {"Fork", &type_ai},
    {"Inventory", &type_ai},
    {"Look", &type_ai},
    {"Connect_nbr", &type_ai},
    {"Incantation", &type_ai},
    {"Broadcast", &type_ai},
    {"msz", &type_gui},
    {"bct", &type_gui},
    {"mct", &type_gui},
    {"tna", &type_gui},
    {"ppo", &type_gui},
    {"plv", &type_gui},
    {"pin", &type_gui},
    {"sgt", &type_gui},
    {"sst", &type_gui},
    {"NULL", NULL}
};

static int path_type(zappy_server_t *zappy, char *cmd)
{
    if (zappy == NULL || cmd == NULL)
        return ERROR;
    for (int i = 0; COMMAND_FUNCS[i].func != NULL; i += 1) {
        if (strncmp(cmd, COMMAND_FUNCS[i].command,
            strlen(COMMAND_FUNCS[i].command)) == 0) {
            COMMAND_FUNCS[i].func(zappy, cmd);
            return OK;
        }
    }
    return OK;
}

static int handle_command(zappy_server_t *zappy, char *command)
{
    if (zappy == NULL || command == NULL)
        return ERROR;
    if (zappy->actual_sockfd == STDIN_FILENO)
        return handle_server_command(zappy, command);
    switch (zappy->clients[zappy->actual_sockfd].type) {
    case UNKNOWN:
        return handle_unknown_command(zappy, command);
    case AI:
        return ai_function(zappy, &zappy->clients[zappy->actual_sockfd],
            command);
    case GUI:
        return handle_gui_command(zappy, command);
    default:
        return path_type(zappy, command);
    }
    return ERROR;
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
    clean_string(buffer);
    lines = splitter(buffer, END_LINE);
    if (lines == NULL)
        return;
    printf("RECEIVED: %s--\n", buffer);
    for (j = 0; lines[j] != NULL; j += 1) {
        handle_command(zappy, lines[j]);
    }
    printf("END handle_command\n");
    free_array(lines);
}
