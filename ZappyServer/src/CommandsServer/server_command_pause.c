/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

void server_command_pause(zappy_server_t *zappy, char *command)
{
    if (zappy == NULL || command == NULL)
        return;
    if (command[0] != '\0')
        return;
    zappy->server_start_game = false;
}
