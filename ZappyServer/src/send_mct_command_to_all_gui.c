/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** send_ppo_command_to_all_gui
*/

#include <zappy_server.h>

static void send_mct_command_to_all_gui_sub(
    zappy_server_t *zappy, int socket)
{
    for (int y = 0; y < zappy->args->height; y += 1) {
        for (int x = 0; x < zappy->args->width; x += 1) {
            display_gui_tile(&zappy->map_tile[y][x], socket);
        }
    }
}

void send_mct_command_to_all_gui(zappy_server_t *zappy)
{
    for (int i = 3; i < zappy->nb_connected_clients; i += 1) {
        if (zappy->clients[i].type == GUI) {
            send_mct_command_to_all_gui_sub(zappy, i);
        }
    }
}
