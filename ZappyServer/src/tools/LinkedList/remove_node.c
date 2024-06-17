/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** remove_node
*/

#include "server_struct.h"

int remove_first_node(message_t *list)
{
    message_t *current = list;
    message_t *next_tmp;

    if (list == NULL)
        return ERROR;
    if (current->next != NULL) {
        next_tmp = current->next;
        free_node(current);
        list = next_tmp;
    } else {
        free_node(current);
        list = NULL;
    }
    return OK;
}