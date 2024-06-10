/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ai_command_right
*/

#include <zappy_server.h>

int ai_command_fork(zappy_server_t *zappy, client_t *client, char *cmd)
{
    if (client == NULL || zappy == NULL || cmd == NULL)
        return ERROR;
    if (cast_action(zappy, client, 42, cmd) == ERROR)
        return ERROR;
    if (check_action(zappy, client) == false)
        return OK;
    send_pfk_command_to_all_gui(zappy, client);
    dprintf(zappy->actual_sockfd, "ok\n");
    return OK;
}
