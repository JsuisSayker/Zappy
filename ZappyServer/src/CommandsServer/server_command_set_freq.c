/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

void server_command_set_freq(zappy_server_t *zappy, char *command)
{
    if (command == NULL || command[0] != ' ')
        return;
    zappy->args->freq = atoi(&command[1]);
}
