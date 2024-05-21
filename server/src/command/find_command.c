/*
** EPITECH PROJECT, 2024
** MyTeams
** File description:
** find_command
*/

#include <stdio.h>
#include <string.h>

#include "server.h"

const struct function_tab_s OPERATORS_FUNCS[] = {
    {"/use", &use},
    {"/user", &user},
    {"/help", &help},
    {"/info", &info},
    {"/users", &users},
    {"/login", &login},
    {"/create", &create},
    {"/logout", &logout},
    {"/subscribe", &subscribe},
    {"/subscribed", &subscribed},
    {"/unsubscribe", &unsubscribe},
    {"/list", &server_list_command},
    {"/send", &server_send_command},
    {"/messages", &server_message_command},
    {NULL, NULL}
};

static int find_command_sub(server_data_t *server, client_server_t *client,
    int j, bool *is_found)
{
    if (server == NULL || client == NULL)
        return ERROR;
    if (strcmp(OPERATORS_FUNCS[j].str, client->command->command) == 0) {
        if (OPERATORS_FUNCS[j].flags(server, client) == ERROR)
            return ERROR;
    }
    *is_found = true;
    return OK;
}

int find_command(server_data_t *server, client_server_t *client)
{
    bool is_found = false;

    if (server == NULL || client == NULL)
        return ERROR;
    for (int j = 0; OPERATORS_FUNCS[j].flags != NULL; j++) {
        if (find_command_sub(server, client, j, &is_found) == ERROR)
            return ERROR;
    }
    if (is_found == false) {
        write(client->socket, "500 Command not found\n", 23);
        return OK;
    }
    return OK;
}
