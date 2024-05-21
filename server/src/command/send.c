/*
** EPITECH PROJECT, 2024
** MyTeams
** File description:
** send
*/

#include "server.h"

static int send_command_response(client_server_t *client,
    personnal_message_t *message)
{
    char *response = NULL;

    if (message == NULL || client == NULL)
        return ERROR;
    append_to_string(&response, "200|/send|");
    append_to_string(&response, message->sender_uuid);
    append_to_string(&response, "|");
    append_to_string(&response, message->message);
    append_to_string(&response, "|");
    append_to_string(&response, message->time);
    append_to_string(&response, "\a\n");
    if (server_response(client->socket, response) == ERROR)
        return ERROR;
    free(response);
    return OK;
}

static int insert_on_data(client_server_t *sender, user_t *user_reciever,
    personnal_message_t *message_sender, personnal_message_t *message_reciever)
{
    if (sender == NULL || user_reciever == NULL || message_sender == NULL
        || message_reciever == NULL)
        return ERROR;
    TAILQ_INSERT_HEAD(&user_reciever->personnal_messages,
        message_reciever, entries);
    TAILQ_INSERT_HEAD(&sender->user->personnal_messages,
        message_sender, entries);
    server_event_private_message_sended(message_reciever->sender_uuid,
    message_reciever->receiver_uuid, message_reciever->message);
    return OK;
}

int server_send_command_sub(client_server_t *sender, client_server_t *reciever,
    user_t *user_reciever)
{
    personnal_message_t *message_sender = NULL;
    personnal_message_t *message_reciever = NULL;

    message_reciever =
        create_personnal_message(sender->command->params->message_body,
        sender->user->uuid, user_reciever->uuid, sender->socket);
    message_sender =
        create_personnal_message(sender->command->params->message_body,
        sender->user->uuid, user_reciever->uuid, sender->socket);
    if (message_reciever == NULL || message_sender == NULL)
        return OK;
    if (insert_on_data(sender, user_reciever, message_sender, message_reciever)
        == ERROR)
        return ERROR;
    if (reciever != NULL){
        send_command_response(reciever, message_reciever);
    }
    return OK;
}

int server_send_command(server_data_t *server, client_server_t *client)
{
    user_t *user = NULL;
    client_server_t *client_reciever = NULL;

    if (server == NULL || client == NULL)
        return ERROR;
    if (client->is_logged == false){
        write(client->socket, "401|/send|Your not logged\a\n\0", 29);
        return OK;
    }
    user = get_user_by_uuid(server, client->command->params->user_uuid);
    if (user == NULL) {
        send_user_not_found(client);
        return OK;
    }
    client_reciever = client_is_connected(server, user);
    if (server_send_command_sub(client, client_reciever, user) == ERROR)
        return ERROR;
    return OK;
}
