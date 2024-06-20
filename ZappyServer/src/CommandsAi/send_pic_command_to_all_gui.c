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
        if (zappy->clients[i].type == IA){
            sprintf(tmb_buffer, " #%d", client->client_number);
            realloc_and_strcat(&buffer, tmb_buffer);
            memset(tmb_buffer, 0, 1000);
        }
    }
    for (int i = 3; i < zappy->nb_connected_clients; i += 1) {
        if (zappy->clients[i].type == GUI) {
            dprintf(i, "pic %d %d %d %s\n", client->pos.x, client->pos.y,
            client->level, buffer);
        }
    }
    free(buffer);
}
