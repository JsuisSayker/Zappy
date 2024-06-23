/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

void tp_client(zappy_server_t *zappy, int client_number, int x, int y)
{
    for (int i = 3; i < zappy->nb_connected_clients; i += 1) {
        if (zappy->clients[i].client_number == client_number) {
            zappy->clients[i].pos.x = x;
            zappy->clients[i].pos.y = y;
        }
    }
}

void server_command_tp(zappy_server_t *zappy, char *command)
{
    int client_number = 0;
    int x = 0;
    int y = 0;
    char **command_array = NULL;

    if (command == NULL || command[0] != ' ')
        return;
    command_array = splitter(command, " ");
    if (get_len_char_tab(command_array) != 3)
        return;
    client_number = atoi(command_array[0]);
    x = atoi(command_array[1]);
    y = atoi(command_array[2]);
    if (client_number > 0 && x >= 0 && x < zappy->args->width && y >= 0 &&
        y < zappy->args->height) {
        tp_client(zappy, client_number, x, y);
    }
}
