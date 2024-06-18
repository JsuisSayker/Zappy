/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ai_command_inventory
*/

#include <zappy_server.h>

void send_inventory(inventory_t *inventory, int socket)
{
    dprintf(socket,
        "[food %d, linemate %d, deraumere %d, sibur %d, mendiane %d, phiras "
        "%d, thystame %d]\n",
        inventory->food, inventory->linemate, inventory->deraumere,
        inventory->sibur, inventory->mendiane, inventory->phiras,
        inventory->thystame);
}

int ai_command_inventory(zappy_server_t *zappy, client_t *client, char *cmd)
{
    if (client == NULL || zappy == NULL || cmd == NULL)
        return ERROR;
    if (cast_action(zappy, client, 1, cmd) == ERROR)
        return ERROR;
    if (check_action(zappy, client) == false)
        return OK;
    send_inventory(&client->inventory, zappy->actual_sockfd);
    printf("Inventory\n");
    send_pin_command_to_all_gui(zappy, client);
    printf("Pin\n");
    return OK;
}
