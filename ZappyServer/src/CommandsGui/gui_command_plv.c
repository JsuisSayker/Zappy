/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

void gui_command_plv(zappy_server_t *zappy, char *command)
{
    int client_nb = 0;

    if (command[0] != ' ' && command[1] != '#')
        return send_sbp_command_to_all_gui(zappy);
    command = &command[2];
    client_nb = atoi(command);
    if (client_nb < 3)
        return send_sbp_command_to_all_gui(zappy);
    dprintf(zappy->actual_sockfd, "plv %d %d\n", client_nb,
        zappy->clients[client_nb].level);
}
