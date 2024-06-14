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
    for (int i = 0; i < FD_SETSIZE; i++) {
        if (zappy->clients[i].client_number == atoi(command)) {
            dprintf(i, "dead\n");
            send_pdi_command_to_all_gui(zappy, &zappy->clients[i]);
            FD_CLR(i, &zappy->fd.save_input);
            zappy->clients[i].type = UNKNOWN;
            return;
        }
    }
}
