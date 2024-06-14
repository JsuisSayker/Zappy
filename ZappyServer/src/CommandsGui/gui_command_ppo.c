/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

void gui_command_ppo(zappy_server_t *zappy, char *command)
{
    int client_nb = 0;

    if (command[0] != ' ' && command[1] != '#') {
        send_sbp_command_to_all_gui(zappy);
        return;
    }
    command = &command[1];
    client_nb = atoi(command);
    if (client_nb < 3)
        return;
    dprintf(zappy->actual_sockfd, "ppo %d %d %d %d\n",
    zappy->clients[client_nb].client_number, zappy->clients[client_nb].pos.x,
    zappy->clients[client_nb].pos.y, zappy->clients[client_nb].pos.direction);
}
