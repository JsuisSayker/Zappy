/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

void server_command_kill(zappy_server_t *zappy, char *command)
{
    if (zappy == NULL || command == NULL)
        return;
    if (command[0] != ' ')
        return;
    command = &command[1];
    if (atoi(command) < 0)
        return;
    dprintf(zappy->actual_sockfd, "dead\n");
    send_pdi_command_to_all_gui(zappy, atoi(command));
    FD_CLR(zappy->actual_sockfd, &zappy->fd.save_input);
    for (int i = 0; zappy->clients[i] != NULL; i++) {
        if (zappy->clients[i].client_number == atoi(command)) {
            zappy->clients[i].type = UNKNOWN;
            return;
        }
    }
}
