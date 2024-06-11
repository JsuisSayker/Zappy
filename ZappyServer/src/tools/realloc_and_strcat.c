/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** realloc_and_strcat
*/

#include <zappy_server.h>

void realloc_and_strcat(char **message, char *str)
{
    *message = realloc(*message, sizeof(char) * (strlen(*message) +
        strlen(str)) + 1);
    *message = strcat(*message, str);
}