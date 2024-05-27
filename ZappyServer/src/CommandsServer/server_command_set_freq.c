/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

void server_command_set_freq(zappy_server_t *zappy, char *command)
{
    int new_freq = 0;
    char **command_array = NULL;

    if (command == NULL || command[0] != ' ')
        return;
    command_array = splitter(command, " ");
    if (get_len_char_tab(command_array) != 4) {
        free_array(command_array);
        return;
    }
    new_freq = atoi(command_array[0]);
    zappy->args->freq = new_freq;
    free_array(command_array);
}
