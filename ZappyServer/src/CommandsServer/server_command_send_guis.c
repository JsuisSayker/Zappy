/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

void server_command_send_guis(zappy_server_t *zappy, char *command)
{
    if (command == NULL || command[0] != ' ')
        return;
    for (int i = 0; i < FD_SETSIZE; i += 1) {
        if (zappy->clients[i].type == GUI) {
            dprintf(i, "smg %s\n", &command[1]);
        }
    }
}
