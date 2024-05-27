/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** is_valid_resource
*/

#include <zappy_server.h>

bool is_valid_resource(char *resource)
{
    if (strcmp(resource, "food") == 0)
        return true;
    if (strcmp(resource, "linemate") == 0)
        return true;
    if (strcmp(resource, "deraumere") == 0)
        return true;
    if (strcmp(resource, "sibur") == 0)
        return true;
    if (strcmp(resource, "mendiane") == 0)
        return true;
    if (strcmp(resource, "phiras") == 0)
        return true;
    if (strcmp(resource, "thystame") == 0)
        return true;
    return false;
}
