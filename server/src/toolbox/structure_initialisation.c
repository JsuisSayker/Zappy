/*
** EPITECH PROJECT, 2024
** MyTeams
** File description:
** structure_initialisation
*/

#include "server.h"

static int init_information(user_t **new_user, char *name)
{
    char *uuid = NULL;

    if (name == NULL || *new_user == NULL)
        return ERROR;
    if (strcpy((*new_user)->username, name) == NULL)
        return ERROR;
    if (strcpy((*new_user)->description, "No description\0") == NULL)
        return ERROR;
    uuid = generate_uuid();
    if (uuid == NULL)
        return ERROR;
    if (strcpy((*new_user)->uuid, uuid) == NULL){
        free(uuid);
        return ERROR;
    }
    free(uuid);
    return OK;
}

int user_initialisation(user_t **new_user, char *name, int socket)
{
    if (name == NULL || *new_user == NULL)
        return ERROR;
    if (strchr(name, '|') != NULL){
        write(socket, "500 Username can't contain '|'\a\n\0", 30);
        return ERROR;
    }
    if (init_information(new_user, name) == ERROR)
        return ERROR;
    (*new_user)->user_connected = 1;
    (*new_user)->entries.tqe_next = NULL;
    (*new_user)->entries.tqe_prev = NULL;
    TAILQ_INIT(&(*new_user)->personnal_messages);
    return OK;
}

personnal_message_t *create_personnal_message(char *message,
    char *sender_uuid, char *receiver_uuid, int socket)
{
    personnal_message_t *new_message = calloc(sizeof(personnal_message_t), 1);

    if (new_message == NULL)
        return NULL;
    if (strlen(message) > MAX_BODY_LENGTH){
        free(new_message);
        write(socket, "500|Message too long\a\n\0", 24);
        return NULL;
    }
    if (strcpy(new_message->message, message) == NULL)
        return NULL;
    if (strcpy(new_message->sender_uuid, sender_uuid) == NULL)
        return NULL;
    if (strcpy(new_message->receiver_uuid, receiver_uuid) == NULL)
        return NULL;
    if (get_time(&new_message->time) == ERROR)
        return NULL;
    new_message->is_read = false;
    return new_message;
}
