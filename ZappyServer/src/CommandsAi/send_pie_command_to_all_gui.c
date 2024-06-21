/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** send_ppo_command_to_all_gui
*/

#include <zappy_server.h>

void send_pie_command_to_all_gui(zappy_server_t *zappy, client_t *client,
    int result)
{
    for (int i = 3; i < zappy->nb_connected_clients; i += 1) {
        if (zappy->clients[i].type == GUI) {
            dprintf(i, "pie %d %d %d\n", client->pos.x, client->pos.y,
            result);
        }
    }
}
