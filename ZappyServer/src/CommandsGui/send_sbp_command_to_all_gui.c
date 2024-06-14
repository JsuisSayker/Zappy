/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** send_ppo_command_to_all_gui
*/

#include <zappy_server.h>

void send_sbp_command_to_all_gui(zappy_server_t *zappy)
{
    for (int i = 3; i < FD_SETSIZE; i += 1) {
        if (zappy->clients[i].type == GUI)
            dprintf(i, "sbp\n");
    }
}
