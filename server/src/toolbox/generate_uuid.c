/*
** EPITECH PROJECT, 2024
** MyTeams
** File description:
** generate_uuid
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

char *generate_uuid(void)
{
    char *uuid = malloc(sizeof(char) * 37);
    char *charset = "0123456789abcdefghijklmnopqrstuvwxyz";
    int i = 0;

    if (uuid == NULL)
        return NULL;
    srand(time(NULL));
    for (i = 0; i < 36; i++) {
        if (i == 8 || i == 13 || i == 18 || i == 23) {
            uuid[i] = '-';
        } else {
            uuid[i] = charset[rand() % 36];
        }
    }
    uuid[i] = '\0';
    return uuid;
}
