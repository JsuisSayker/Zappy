/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

static int is_valid_resource(char *ressource)
{
    if (strcmp(ressource, "food") == 0)
        return true;
    if (strcmp(ressource, "linemate") == 0)
        return true;
    if (strcmp(ressource, "deraumere") == 0)
        return true;
    if (strcmp(ressource, "sibur") == 0)
        return true;
    if (strcmp(ressource, "mendiane") == 0)
        return true;
    if (strcmp(ressource, "phiras") == 0)
        return true;
    if (strcmp(ressource, "thystame") == 0)
        return true;
    return false;
}

void server_command_tp(UNUSED zappy_server_t *zappy, UNUSED char *command)
{
    char **command_array = NULL;
    int x = 0;
    int y = 0;
    char *resource = NULL;

    if (command == NULL || command[0] != ' ')
        return;
    command_array = splitter(command, " ");
    if (get_len_char_tab(command_array) != 3)
        return;
    resource = command_array[0];
    x = atoi(command_array[1]);
    y = atoi(command_array[2]);
    if (is_valid_resource == true && x >= 0 && x < zappy->args->width && y >= 0 && y < zappy->args->height) {
        dprintf(zappy->actual_sockfd, "ok\n");
    } else {
        dprintf(zappy->actual_sockfd, "ko\n");
    }
    
}
