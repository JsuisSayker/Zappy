/*
** EPITECH PROJECT, 2024
** MyTeams
** File description:
** client_is_connected
*/

#include "server.h"

client_server_t *client_is_connected(server_data_t *server, user_t *user)
{
    client_server_t *client = NULL;

    if (user == NULL || server == NULL)
        return NULL;
    if (user->user_connected < 0)
        return NULL;
    client = server->clients.tqh_first;
    TAILQ_FOREACH(client, &server->clients, entries){
        if (strcmp(client->user->uuid, user->uuid) == 0)
            return client;
    }
    return OK;
}
