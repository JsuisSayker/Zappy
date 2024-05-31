/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ai_command_right
*/

#include <zappy_server.h>

void send_look_command_to_client(zappy_server_t *zappy, client_t *client)
{
    int x = client->pos.x;
    int y = client->pos.y;
    int i = 0;
    int j = 0;

    dprintf(zappy->actual_sockfd, "[");
    for (int k = 0; k < client->level + 1; k++) {
        for (i = x - k; i <= x + k; i++) {
            for (j = y - k; j <= y + k; j++) {
                if (i < 0)
                    i = zappy->args->width - 1;
                if (j < 0)
                    j = zappy->args->height - 1;
                if (i >= zappy->args->width)
                    i -= zappy->args->width;
                if (j >= zappy->args->height)
                    j -= zappy->args->height;
                for (int l = 0; l < FD_SETSIZE; l++){
                    if (zappy->clients[l].type == IA &&
                        zappy->clients[l].pos.x == i &&
                        zappy->clients[l].pos.y == j)
                        dprintf(zappy->actual_sockfd, " player");
                }
                // dprintf(zappy->actual_sockfd, "%s", zappy->map_tile[j][i].
                //     inventory_to_string(&zappy->map_tile[j][i].inventory));
                // if (i != x + k || j != y + k)
                //     dprintf(zappy->actual_sockfd, ",");
            }
        }
    }
    dprintf(zappy->actual_sockfd, "]\n");
}

int ai_command_look(zappy_server_t *zappy, client_t *client, char *cmd)
{
    if (client == NULL || zappy == NULL || cmd == NULL)
        return ERROR;
    if (cast_action(zappy, client, 7, cmd) == ERROR)
        return ERROR;
    if (check_action(zappy, client) == false)
        return OK;
    send_look_command_to_client(zappy, client);
    return OK;
}
