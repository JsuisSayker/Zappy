/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

void gui_command_plv(zappy_server_t *zappy, char *command)
{
    int client_nb = 0;

    if (command[0] != ' ' && command[1] != '#')
        return;
    command = &command[1];
    client_nb = atoi(command);
    if (client_nb < 3)
        return;
    dprintf(zappy->actual_sockfd, "plv %d %d\n", client_nb,
        zappy->clients[client_nb].level);
}
