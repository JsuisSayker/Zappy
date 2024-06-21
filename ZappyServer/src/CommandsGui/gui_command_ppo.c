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

    if (command[0] != ' ' && command[1] != '#')
        return send_sbp_command_to_all_gui(zappy);
    command = &command[2];
    client_nb = atoi(command);
    if (client_nb < 3)
        return send_sbp_command_to_all_gui(zappy);
    dprintf(zappy->actual_sockfd, "ppo #%d %d %d %s\n",
    zappy->clients[client_nb].client_number, zappy->clients[client_nb].pos.x,
    zappy->clients[client_nb].pos.y,
    direction_string(zappy->clients[client_nb].pos.direction));
}
