/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

void gui_command_gui_disconnected(zappy_server_t *zappy, char *command)
{
    if (command[0] != '\0')
        return send_sbp_command_to_all_gui(zappy);
    close(zappy->actual_sockfd);
    FD_CLR(zappy->actual_sockfd, &zappy->fd.save_input);
    close(zappy->actual_sockfd);
    zappy->clients[zappy->actual_sockfd].type = UNKNOWN;
}
