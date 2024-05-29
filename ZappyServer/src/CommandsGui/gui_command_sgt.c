/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

void gui_command_sgt(zappy_server_t *zappy, UNUSED char *command)
{
    if (command == NULL || command[0] != '\0')
        return;
    dprintf(zappy->actual_sockfd, "sgt %.0f\n", zappy->args->freq);
}
