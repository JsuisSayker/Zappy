/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** direction_function
*/

#include <zappy_server.h>

char *direction_string(direction_t orientation)
{
    if (orientation == NORTH)
        return "N";
    if (orientation == EAST)
        return "E";
    if (orientation == SOUTH)
        return "S";
    if (orientation == WEST)
        return "W";
    return "U";
}