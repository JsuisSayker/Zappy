/*
** EPITECH PROJECT, 2024
** My_teams
** File description:
** send_user_not_found
*/

#include "server.h"

void send_user_not_found(client_server_t *client)
{
    char *response = NULL;

    append_to_string(&response, "404|/user|");
    append_to_string(&response, client->command->params->user_uuid);
    append_to_string(&response, "| user not found");
    append_to_string(&response, "\a\n");
    write(client->socket, response, strlen(response));
    free(response);
}
