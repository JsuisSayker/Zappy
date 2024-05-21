/*
** EPITECH PROJECT, 2024
** MyTeams
** File description:
** users_function
*/

#include "server.h"

static int users_command_response(list_user_t *list, int socket)
{
    char *message = NULL;

    append_to_string(&message, "200|/users|");
    if (list == NULL)
        return ERROR;
    for (list_user_t *tmp = list; tmp != NULL; tmp = tmp->next) {
        append_to_string(&message, tmp->username);
        append_to_string(&message, "|");
        append_to_string(&message, tmp->uuid);
        append_to_string(&message, "|");
        append_to_string(&message, tmp->is_connected);
        if (tmp->next != NULL)
            append_to_string(&message, "|");
    }
    append_to_string(&message, "\a\n");
    if (server_response(socket, message) == ERROR)
        return ERROR;
    free(message);
    return OK;
}

static int get_list(list_user_t **list, server_data_t *server)
{
    user_t *tmp;

    if (server == NULL || list == NULL)
        return ERROR;
    tmp = server->users.tqh_first;
    TAILQ_FOREACH(tmp, &server->users, entries) {
        (*list) = add_node_in_list((*list), tmp->username,
        tmp->uuid, tmp->user_connected);
    }
    return OK;
}

int users(server_data_t *server, client_server_t *client)
{
    list_user_t *list = NULL;

    if (client == NULL || server == NULL)
        return ERROR;
    if (client->is_logged == false){
        write(client->socket, "401|/users|Your not logged\a\n\0", 30);
        return OK;
    }
    if (get_list(&list, server) == ERROR)
        return ERROR;
    if (list == NULL){
        write(client->socket, "404|/users|No users found\a\n\0", 29);
        return OK;
    }
    if (users_command_response(list, client->socket) == ERROR)
        return ERROR;
    free_list(list);
    return OK;
}

static int user_command_response(user_t *user, int socket)
{
    char *message = NULL;

    append_to_string(&message, "200|/user|");
    append_to_string(&message, user->username);
    append_to_string(&message, "|");
    append_to_string(&message, user->uuid);
    append_to_string(&message, "|");
    if (user->user_connected > 0)
        append_to_string(&message, "1");
    else
        append_to_string(&message, "0");
    append_to_string(&message, "\a\n");
    if (server_response(socket, message) == ERROR)
        return ERROR;
    free(message);
    return OK;
}

int user(server_data_t *server, client_server_t *client)
{
    user_t *tmp;

    if (server == NULL || client == NULL)
        return ERROR;
    if (client->is_logged == false){
        write(client->socket, "401|/user|Your not logged\a\n\0", 29);
        return OK;
    }
    tmp = get_user_by_uuid(server, client->command->params->user_uuid);
    if (tmp == NULL) {
        send_user_not_found(client);
        return OK;
    }
    if (user_command_response(tmp, client->socket) == ERROR)
        return ERROR;
    return OK;
}
