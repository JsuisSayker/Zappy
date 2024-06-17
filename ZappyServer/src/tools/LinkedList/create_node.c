/*
** EPITECH PROJECT, 2023
** create_node
** File description:
** create_node
*/

#include <unistd.h>
#include <stdlib.h>

#include "zappy_server.h"

message_t *create_node(char *cmd, ai_position_t pos)
{
    message_t *new_node = malloc(sizeof(message_t));

    if (!new_node)
        return NULL;
    new_node->message = strdup(cmd);
    new_node->pos.x = pos.x;
    new_node->pos.y = pos.y;
    new_node->next = NULL;
    return new_node;
}
