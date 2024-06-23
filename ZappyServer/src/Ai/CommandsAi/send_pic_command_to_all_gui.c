/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** send_ppo_command_to_all_gui
*/

#include <zappy_server.h>

static bool condition(client_t *client, client_t *client2)
{
    return (client2->type == AI && client->pos.x == client2->pos.x &&
    client->pos.y == client2->pos.y &&
    client->level == client2->level);
}

void send_pic_command_to_all_gui(zappy_server_t *zappy, client_t *client)
{
    char *buffer = calloc(sizeof(char), 1);
    char tmb_buffer[1000] = {0};

    for (int i = 3; i < zappy->nb_connected_clients; i += 1) {
        if (condition(client, &zappy->clients[i]) == true) {
            sprintf(tmb_buffer, " #%d", zappy->clients[i].client_number);
            realloc_and_strcat(&buffer, tmb_buffer);
            memset(tmb_buffer, 0, 1000);
        }
    }
    for (int i = 3; i < zappy->nb_connected_clients; i += 1) {
        if (zappy->clients[i].type == GUI) {
            dprintf(i, "pic %d %d %d%s\n", client->pos.x, client->pos.y,
            client->level, buffer);
        }
    }
    free(buffer);
}
