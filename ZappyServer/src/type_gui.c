/*
** EPITECH PROJECT, 2024
** server_functions
** File description:
** server_functions
*/

#include "zappy_server.h"

int type_ai(zappy_server_t *zappy, char *cmd)
{
    client_t *client;

    if (zappy == NULL || cmd == NULL)
        return ERROR;
    client = &zappy->clients[zappy->actual_sockfd];
    client->type = IA;
    printf("Je suis dans type_ai %d\n", client->type);
    ai_function(zappy, client, cmd);
    return OK;
}
