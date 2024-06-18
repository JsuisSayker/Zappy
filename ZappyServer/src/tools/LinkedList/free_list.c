/*
** EPITECH PROJECT, 2023
** B-CPE-200-BDX-2-1-lemin-marc.mendia
** File description:
** free_list
*/

#include <stdlib.h>
#include <unistd.h>

#include "server_struct.h"

void free_node(message_t *node)
{
    if (node->message != NULL) {
        free(node->message);
    }
    node->next = NULL;
    free(node);
    node = NULL;
}

void free_list(message_t *list)
{
    message_t *list_tmp = NULL;
    message_t *current = list;

    while (current != NULL) {
        list_tmp = current;
        current = current->next;
        free_node(list_tmp);
    }
}
