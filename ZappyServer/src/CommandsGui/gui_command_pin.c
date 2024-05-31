/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

static void display_player_inventory(client_t *player, int socket)
{
    dprintf(socket, "pin %d %d %d %d %d %d %d %d %d %d\n",
        player->client_number, player->pos.x, player->pos.y,
        player->inventory.food, player->inventory.linemate,
        player->inventory.deraumere, player->inventory.sibur,
        player->inventory.mendiane, player->inventory.phiras,
        player->inventory.thystame);
}

void gui_command_pin(zappy_server_t *zappy, char *command)
{
    int client_nb = 0;

    if (command[0] != ' ' && command[1] != '#')
        return;
    client_nb = atoi(&command[1]);
    for (int i = 0; i < FD_SETSIZE; i += 1) {
        if (zappy->clients[i].client_number == client_nb &&
            zappy->clients[i].type == IA) {
            display_player_inventory(&zappy->clients[i], zappy->actual_sockfd);
            return;
        }
    }
}
