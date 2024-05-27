/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ia_command_forward
*/

#include <zappy_server.h>

static int check_direction(char *direction)
{
    if (strcmp(direction, "North") == 0)
        return OK;
    if (strcmp(direction, "East") == 0)
        return OK;
    if (strcmp(direction, "South") == 0)
        return OK;
    if (strcmp(direction, "West") == 0)
        return OK;
    return ERROR;
}

int ia_command_forward(zappy_server_t *zappy, client_t *client, char *cmd)
{
    char **tab = NULL;
    int len = 0;

    if (client == NULL || zappy == NULL || cmd == NULL)
        return ERROR;
    if (cast_action(zappy, client, 7) == ERROR)
        return ERROR;
    if (check_action(zappy, client) == false)
        return OK;
    tab = my_str_to_word_array(cmd, " ");
    if (tab == NULL)
        return ERROR;
    len = my_tab_len(tab);
    if (len != 2)
        return ERROR;
    if (check_direction(tab[1]) == ERROR)
        return ERROR;
    return OK;
}
