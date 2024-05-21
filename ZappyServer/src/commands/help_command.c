/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** help_command
*/

#include "zappy_server.h"

static int handle_error(zappy_server_t *zappy_server, char *command)
{
    if (zappy_server->clients[zappy_server->actual_sockfd].user == NULL) {
        dprintf(zappy_server->actual_sockfd, "502|Unauthorized action%s%s",
            END_LINE, END_STR);
        return KO;
    }
    if (strlen(command) != 0) {
        dprintf(zappy_server->actual_sockfd, "500|Internal Server Error\n");
        dprintf(zappy_server->actual_sockfd, END_STR);
        return KO;
    }
    return OK;
}

void help_command(
    zappy_server_t *zappy_server, char __attribute__((unused)) * command)
{
    if (handle_error(zappy_server, command) == KO) {
        return;
    }
    dprintf(zappy_server->actual_sockfd, "214|/help%s", END_LINE);
    dprintf(zappy_server->actual_sockfd, "COMMANDS:\n/help\n/login [\"userna");
    dprintf(zappy_server->actual_sockfd, "me\"]\n/logout\n/users\n/user [\"u");
    dprintf(zappy_server->actual_sockfd, "ser_uuid\"]\n/send [\"user_uuid\"]");
    dprintf(zappy_server->actual_sockfd, " [\"message_body\"]\n/messages[\"u");
    dprintf(zappy_server->actual_sockfd, "ser_uuid\"]\n/subscribe [\"team_uu");
    dprintf(zappy_server->actual_sockfd, "id\"]\n/subscribed\n/unsubscribe [");
    dprintf(zappy_server->actual_sockfd, "\"team_uuid\"]\n/use [\"team_uuid");
    dprintf(zappy_server->actual_sockfd, "\"]\n/create [\"name\"] [\"te");
    dprintf(zappy_server->actual_sockfd, "am_description\"]\n/list\n/info [");
    dprintf(zappy_server->actual_sockfd, "\"team_uuid\"]\n");
    dprintf(zappy_server->actual_sockfd, END_STR);
}
