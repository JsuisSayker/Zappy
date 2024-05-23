/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

void server_command_map(UNUSED zappy_server_t *zappy, UNUSED char *command)
{
    display_map_tile(zappy->map_tile);
}
