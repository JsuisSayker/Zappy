/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

void gui_command_pin(zappy_server_t *zappy, char *command)
{
    int client_nb = 0;

    if (command[0] != ' ' && command[1] != '#')
        return;
    command = &command[1];
    client_nb = atoi(command);
    if (client_nb < 3)
        return;
    dprintf(zappy->actual_sockfd, "pin %d %d %d %d %d %d %d %d %d %d\n",
        client_nb,
        zappy->clients[client_nb].pos.x, zappy->clients[client_nb].pos.y,
        zappy->clients[client_nb].inventory.food, zappy->clients[client_nb].
        inventory.linemate, zappy->clients[client_nb].inventory.deraumere,
        zappy->clients[client_nb].inventory.sibur, zappy->clients[client_nb].
        inventory.mendiane, zappy->clients[client_nb].inventory.phiras,
        zappy->clients[client_nb].inventory.thystame);
}
