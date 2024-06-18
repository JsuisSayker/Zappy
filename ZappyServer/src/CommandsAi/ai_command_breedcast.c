/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ai_command_breedcast
*/

#include "zappy_server.h"

static int spreads_breedcast(zappy_server_t *zappy, client_t *client,
    char *message)
{
    if (zappy == NULL || client == NULL)
        return ERROR;
    for (int i = 0; i < zappy->nb_connected_clients; i ++){
        printf("zappy number %d\n", zappy->clients[i].client_number);
        printf("client number %d\n", client->client_number);
        if (zappy->clients[i].type == IA && client->client_number !=
        zappy->clients[i].client_number) {
            zappy->clients[i].message_receive = add_node_in_list(
            zappy->clients[i].message_receive, message, client->pos);
            printf("ta putain de grandmeme\n");
        }
    }
    dprintf(zappy->actual_sockfd, "ok\n");
    return OK;
}

int ai_command_breedcast(zappy_server_t *zappy, client_t *client,
    char *cmd)
{
    char *message = NULL;

    if (client == NULL || zappy == NULL || cmd == NULL)
        return ERROR;
    if (cast_action(zappy, client, 7, cmd) == ERROR)
        return ERROR;
    if (check_action(zappy, client) == false)
        return OK;
    message = cmd[9] == ' ' ? &cmd[10] : &cmd[9];
    if (spreads_breedcast(zappy, client, message) == ERROR) {
        dprintf(zappy->actual_sockfd, "ko\n");
        return ERROR;
    }
    return OK;
}
