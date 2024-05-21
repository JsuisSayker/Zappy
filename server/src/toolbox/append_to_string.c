/*
** EPITECH PROJECT, 2024
** MyTeams
** File description:
** append_to_string
*/

#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "macro.h"

int append_to_string(char **str, char *to_append)
{
    int len = 0;
    char *new_str = NULL;

    if (to_append == NULL)
        return ERROR;
    if (*str == NULL){
        *str = strdup(to_append);
        return OK;
    }
    len = strlen(*str) + strlen(to_append);
    new_str = malloc(sizeof(char) * len + 1);
    strcpy(new_str, *str);
    strcat(new_str, to_append);
    new_str[len] = '\0';
    if (*str != NULL)
        free(*str);
    *str = new_str;
    return OK;
}
