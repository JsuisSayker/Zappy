/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

void gui_command_sst(zappy_server_t *zappy, char *command)
{
    if (command == NULL || command[0] != ' ')
        return;
    dprintf(zappy->actual_sockfd, "sst %d\n", zappy->args->freq);
    send_sgt_command_to_all_gui(zappy);
}
