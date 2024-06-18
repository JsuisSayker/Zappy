/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ai_command_breadcast
*/

#include "zappy_server.h"

static int spreads_breadcast(zappy_server_t *zappy, client_t *client,
    char *message)
{
    message_t *new_message = malloc(sizeof(message_t));

    if (new_message == NULL)
        return ERROR;
    new_message->message = strdup(message);
    new_message->pos.x = client->pos.x;
    new_message->pos.y = client->pos.y;
    if (zappy == NULL || client == NULL)
        return ERROR;
    for (int i = 0; i < zappy->nb_connected_clients; i ++){
        if (client->client_number != zappy->clients[i].client_number
            && zappy->clients[i].type == IA){
                read_message_receive(zappy,
                    &zappy->clients[i], new_message, i);
        }
    }
    free_string(&new_message->message);
    free(new_message);
    dprintf(zappy->actual_sockfd, "ok\n");
    return OK;
}

int ai_command_breadcast(zappy_server_t *zappy, client_t *client,
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
    if (spreads_breadcast(zappy, client, message) == ERROR) {
        dprintf(zappy->actual_sockfd, "ko\n");
        return ERROR;
    }
    return OK;
}
