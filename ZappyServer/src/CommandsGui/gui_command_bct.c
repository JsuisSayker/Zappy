/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

static int error_command_bct(zappy_server_t *zappy, char **command_args)
{
    if (command_args == NULL || command_args[0] == NULL ||
        command_args[1] == NULL || command_args[2] != NULL) {
        send_sbp_command_to_all_gui(zappy);
        free_array(command_args);
        return KO;
    }
    return OK;
}

void gui_command_bct(zappy_server_t *zappy, char *command)
{
    int x = 0;
    int y = 0;
    char **command_args = NULL;

    if (command[0] != ' ')
        return send_sbp_command_to_all_gui(zappy);
    command_args = splitter(command, " ");
    if (error_command_bct(zappy, command_args) == KO)
        return;
    x = atoi(command_args[0]);
    y = atoi(command_args[1]);
    if (x >= 0 && x < zappy->args->width && y >= 0 && y <
        zappy->args->height) {
        display_gui_tile(&zappy->map_tile[y][x], zappy->actual_sockfd);
    } else {
        send_sbp_command_to_all_gui(zappy);
    }
    free_array(command_args);
}
