/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** send_ppo_command_to_all_gui
*/

#include <zappy_server.h>

void send_pgt_command_to_all_gui(zappy_server_t *zappy, client_t *client,
    int nb_resource)
{
    for (int i = 3; i < zappy->nb_connected_clients; i += 1) {
        if (zappy->clients[i].type == GUI) {
            dprintf(i, "pgt %d %d\n", client->client_number,
                nb_resource);
        }
    }
}
