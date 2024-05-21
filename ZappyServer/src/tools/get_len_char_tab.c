/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** get_nb_args
*/

#include <stdlib.h>

int get_len_char_tab(char **command)
{
    int count = 0;

    if (command == NULL) {
        return 0;
    }
    for (int count = 0; command[count] != NULL; count += 1)
        ;
    return count;
}
