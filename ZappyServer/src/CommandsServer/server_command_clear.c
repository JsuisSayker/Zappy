/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

void server_command_clear(UNUSED zappy_server_t *zappy, UNUSED char *command)
{
    write(STDOUT_FILENO, CLEAR_SCREEN, strlen(CLEAR_SCREEN));
}
