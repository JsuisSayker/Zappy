/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** send_pdr_command_to_all_gui
*/

#include <zappy_server.h>

void send_pdr_command_to_all_gui(zappy_server_t *zappy, client_t *client,
    int nb_resource)
{
    for (int i = 3; i < FD_SETSIZE; i += 1) {
        if (zappy->clients[i].type == GUI) {
            dprintf(i, "pdr %d %d\n", client->client_number, nb_resource);
        }
    }
}
