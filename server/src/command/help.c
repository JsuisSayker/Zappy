/*
** EPITECH PROJECT, 2024
** MyTeams
** File description:
** help
*/

#include "server.h"

int help(server_data_t *server, client_server_t *client)
{
    char *message = NULL;

    append_to_string(&message, "200|/help|");
    append_to_string(&message, "Commands: -/login|-/logout|-/users|");
    append_to_string(&message, "-/user|-/send|-/messages|");
    append_to_string(&message, "-/subscribe|-/subscribed|");
    append_to_string(&message, "-/unsubscribe|-/use|-/create|");
    append_to_string(&message, "-/list|-/info|-/help|");
    append_to_string(&message, "If you need more information about some");
    append_to_string(&message, " command, have a look at our protocol");
    append_to_string(&message, ".txt file\a\n");
    write(client->socket, message, strlen(message));
    free(message);
    return OK;
}
