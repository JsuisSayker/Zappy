/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ai_command_inventory
*/

#include "zappy_server.h"

int ai_command_inventory(zappy_server_t *zappy, client_t *client, char *cmd)
{
    char *inventory_str = NULL;

    if (cast_action(zappy, client, 1, cmd) == ERROR)
        return ERROR;
    if (check_action(zappy, client) == false)
        return OK;
    dprintf(zappy->actual_sockfd, "[food %d, linemate %d, deraumere %d, \
sibur %d, mendiane %d, phiras %d, thystame %d]\n", client->inventory.food,
    client->inventory.linemate, client->inventory.deraumere,
    client->inventory.sibur, client->inventory.mendiane,
    client->inventory.phiras, client->inventory.thystame);
    return OK;
}
