/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** send_ppo_command_to_all_gui
*/

#include <zappy_server.h>

void send_pic_command_to_all_gui(zappy_server_t *zappy, client_t *client)
{
    char *buffer = calloc(sizeof(char), 1);
    char tmb_buffer[1000] = {0};

    for (int i = 3; i < zappy->nb_connected_clients; i += 1) {
        if (zappy->clients[i].type == AI &&
            zappy->clients[i].pos.x == client->pos.x &&
            zappy->clients[i].pos.y == client->pos.y &&
            zappy->clients[i].level == client->level) {
            sprintf(tmb_buffer, " #%d", zappy->clients[i].client_number);
            realloc_and_strcat(&buffer, tmb_buffer);
            memset(tmb_buffer, 0, 1000);
        }
    }
    printf("buffer = %s\n", buffer);
    for (int i = 3; i < zappy->nb_connected_clients; i += 1) {
        if (zappy->clients[i].type == GUI) {
            dprintf(i, "pic %d %d %d%s\n", client->pos.x, client->pos.y,
            client->level, buffer);
        }
    }
    printf("buffer = %s\n", buffer);
    free(buffer);
}
