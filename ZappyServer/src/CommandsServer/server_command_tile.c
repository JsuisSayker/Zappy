/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

void server_command_tile(zappy_server_t *zappy, char *command)
{
    int x = 0;
    int y = 0;
    char **command_args = splitter(command, " ");

    if (command_args == NULL || command_args[0] == NULL ||
        command_args[1] == NULL || command_args[2] != NULL) {
        error_command_argument("/tile", get_len_char_tab(command_args), 2);
        free_array(command_args);
        return;
    }
    x = atoi(command_args[0]);
    y = atoi(command_args[1]);
    if (x >= 0 && x < zappy->args->width && y >= 0 &&
        y < zappy->args->height) {
        display_tile(zappy->map_tile[y][x]);
    } else {
        printf("Invalid coordinates\n");
    }
    free_array(command_args);
}
