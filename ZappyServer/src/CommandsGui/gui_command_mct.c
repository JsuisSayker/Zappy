/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

void gui_command_mct(zappy_server_t *zappy, char *command)
{
    if (command[0] != '\0') {
        send_sbp_command_to_all_gui(zappy);
        return;
    }
    for (int y = 0; y < zappy->args->height; y += 1) {
        for (int x = 0; x < zappy->args->width; x += 1) {
            display_gui_tile(&zappy->map_tile[y][x], zappy->actual_sockfd);
        }
    }
}
