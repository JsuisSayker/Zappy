/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

static void display_client_info(client_t client)
{
    printf("127.0.0.1:%d\n",
        ntohs(client.other_socket_addr.sin_port));
    printf("Type = ");
    if (client.type == AI)
        printf("AI\n");
    if (client.type == GUI)
        printf("GUI\n");
}

void server_command_clients(zappy_server_t *zappy, UNUSED char *command)
{
    printf("================USER================\n");
    for (int i = 3; i < zappy->nb_connected_clients; i += 1){
        if (zappy->clients[i].type != UNKNOWN)
            display_client_info(zappy->clients[i]);
    }
    printf("====================================\n");
}
