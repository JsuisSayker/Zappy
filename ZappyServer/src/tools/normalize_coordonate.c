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
        printf("x0 = %d\n", *x);
        *x = ((zappy->args->width - 1) + *x);
        }
    if (*y < 0){
        printf("y0 = %d\n", *y);
        *y = ((zappy->args->height - 1) + *y);
    }
    if (*x > zappy->args->width - 1){
        printf("x = %d\n", *x);
        *x = (*x - (zappy->args->width - 1));
        }
    if (*y > zappy->args->height - 1){
        printf("y = %d\n", *y);
        *y = (*y - (zappy->args->height - 1));
    }
}
