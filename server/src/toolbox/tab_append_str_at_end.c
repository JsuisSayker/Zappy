/*
** EPITECH PROJECT, 2024
** My_teams
** File description:
** tab_append_str_at_end
*/

#include "server.h"

static char **tab_append_str_at_end_sub(char **tab, char **new_tab, char *str,
    int i)
{
    for (int j = 0; tab[j] != NULL; j++)
        new_tab[j] = strdup(tab[j]);
    new_tab[i] = strdup(str);
    new_tab[i + 1] = NULL;
    free_tab(tab);
    return new_tab;
}

char **tab_append_str_at_end(char **tab, char *str)
{
    int i = 0;
    char **new_tab = NULL;

    if (str == NULL)
        return NULL;
    if (tab == NULL) {
        new_tab = malloc(sizeof(char *) * 2);
        if (new_tab == NULL)
            return NULL;
        new_tab[0] = strdup(str);
        new_tab[1] = NULL;
        return new_tab;
    }
    for (; tab[i] != NULL; i++);
    new_tab = malloc(sizeof(char *) * (i + 2));
    if (new_tab == NULL)
        return NULL;
    return tab_append_str_at_end_sub(tab, new_tab, str, i);
}
