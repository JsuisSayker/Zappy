/*
** EPITECH PROJECT, 2024
** My_teams
** File description:
** accept
*/

#include "server.h"

int accept_client(server_data_t *data)
{
    client_server_t *new_client = calloc(sizeof(client_server_t), 1);
    int client_socket = 0;

    if (data == NULL) {
        perror("Error: data is NULL\n");
        return KO;
    }
    client_socket = accept(data->server_socket, NULL, NULL);
    if (client_socket < 0) {
        perror("Error: accept failed\n");
        return KO;
    }
    FD_SET(client_socket, &data->current_sockets);
    new_client->socket = client_socket;
    TAILQ_INSERT_HEAD(&data->clients, new_client, entries);
    return OK;
}

static char *read_client_msg(int client_socket, server_data_t *server_data)
{
    int nb_bytes = 0;
    char buffer[BUFSIZ];

    nb_bytes = read(client_socket, buffer, sizeof(buffer) - 1);
    if (nb_bytes == -1)
        return NULL;
    if (nb_bytes == 0)
        server_data->client_is_deco = 1;
    buffer[nb_bytes] = '\0';
    return strdup(buffer);
}

char *read_client(server_data_t *server_data, int client_socket)
{
    char *client_msg = NULL;

    if (server_data == NULL) {
        perror("Error: server_data is NULL\n");
        return NULL;
    }
    client_msg = read_client_msg(client_socket, server_data);
    if (client_msg == NULL) {
        perror("Error: read_client_msg failed\n");
        return NULL;
    }
    return client_msg;
}
