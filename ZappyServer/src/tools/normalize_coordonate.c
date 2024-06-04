/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** nomalise_coordonate
*/

#include "zappy_server.h"

void normalize_coordinate(int *x, int *y, zappy_server_t *zappy)
{
    if (*x < 0)
        *x = zappy->args->width - 1;
    if (*y < 0)
        *y = zappy->args->height - 1;
    if (*x >= zappy->args->width)
        *x -= zappy->args->width;
    if (*y >= zappy->args->height)
        *y -= zappy->args->height;
}
