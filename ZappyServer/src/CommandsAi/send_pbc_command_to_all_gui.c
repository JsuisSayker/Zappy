/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** send_ppo_command_to_all_gui
*/

#include <zappy_server.h>

void send_pbc_command_to_all_gui(zappy_server_t *zappy, client_t *client,
    char *message)
{
    for (int i = 3; i < zappy->nb_connected_clients; i += 1) {
        if (zappy->clients[i].type == GUI) {
            dprintf(i, "pbc %d %s\n", client->client_number, message);
        }
    }
}
