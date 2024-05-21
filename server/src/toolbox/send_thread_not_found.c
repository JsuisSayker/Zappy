/*
** EPITECH PROJECT, 2024
** My_teams
** File description:
** send_thread_not_found
*/

#include "server.h"

void send_thread_not_found(client_server_t *client, char *command,
    char *context)
{
    char *response = NULL;

    append_to_string(&response, "602|");
    append_to_string(&response, command);
    append_to_string(&response, "|");
    append_to_string(&response, context);
    append_to_string(&response, "|");
    append_to_string(&response, client->command->params->thread_uuid);
    append_to_string(&response, "| thread not found");
    append_to_string(&response, "\a\n");
    write(client->socket, response, strlen(response));
    free(response);
}
