/*
** EPITECH PROJECT, 2023
** B-CPE-200-BDX-2-1-lemin-marc.mendia
** File description:
** add_node
*/

#include "zappy_server.h"
#include <unistd.h>

message_t *add_node_in_list(message_t *list, char *cmd, ai_position_t pos)
{
    message_t *node = create_node(cmd, pos);
    message_t *next_list = list;

    if (next_list == NULL)
        return node;
    while (next_list->next != NULL)
        next_list = next_list->next;
    next_list->next = node;
    return list;
}
