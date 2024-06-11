/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** free_string
*/

#include <stdlib.h>

#include "macro_server.h"

void free_string(char **str)
{
    if (*str == NULL)
        return;
    free(*str);
    *str = NULL;
}