/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** send_ppo_command_to_all_gui
*/

#include <zappy_server.h>

void send_pin_command_to_all_gui(zappy_server_t *zappy, client_t *client)
{
    for (int i = 3; i < FD_SETSIZE; i += 1) {
        if (zappy->clients[i].type == GUI) {
            dprintf(i, "pin %d %d %d %d %d %d %d %d %d %d\n",
                client->client_number, client->pos.x, client->pos.y,
                client->inventory.food, client->inventory.linemate,
                client->inventory.deraumere, client->inventory.sibur,
                client->inventory.mendiane, client->inventory.phiras,
                client->inventory.thystame);
        }
    }
}
