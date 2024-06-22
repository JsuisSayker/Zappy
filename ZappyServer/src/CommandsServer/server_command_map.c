/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

void server_command_map(zappy_server_t *zappy, UNUSED char *command)
{
    if (zappy == NULL || command == NULL || command[0] != '\0')
        return;
    display_map_tile(zappy->map_tile);
}
