/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** free_array
*/

#include "zappy_server.h"

void free_array(char **array)
{
    if (array == NULL) {
        return;
    }
    for (int i = 0; array[i]; i++) {
        free(array[i]);
    }
    free(array);
}
