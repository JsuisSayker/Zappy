/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

void server_command_clear(zappy_server_t *zappy, char *command)
{
    if (zappy == NULL || command == NULL || command[0] != '\0')
        return;
    write(STDOUT_FILENO, CLEAR_SCREEN, strlen(CLEAR_SCREEN));
}
