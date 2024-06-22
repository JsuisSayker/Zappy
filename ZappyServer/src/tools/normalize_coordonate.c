/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** nomalise_coordonate
*/

#include "zappy_server.h"

void normalize_coordinate(int *x, int *y, zappy_server_t *zappy)
{
    if (*x < 0){
        *x = ((zappy->args->width - 1) + *x);
        }
    if (*y < 0){
        *y = ((zappy->args->height - 1) + *y);
    }
    if (*x > zappy->args->width - 1){
        *x = (*x - (zappy->args->width - 1));
        }
    if (*y > zappy->args->height - 1){
        *y = (*y - (zappy->args->height - 1));
    }
}
