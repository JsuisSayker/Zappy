/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

void gui_command_msz(zappy_server_t *zappy, UNUSED char *command)
{
    if (command[0] != '\0') {
        send_sbp_command_to_all_gui(zappy);
        return;
    }
    dprintf(zappy->actual_sockfd, "msz %d %d\n", zappy->args->width,
        zappy->args->height);
}
