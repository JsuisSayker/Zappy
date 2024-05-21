/*
** EPITECH PROJECT, 2024
** My_teams
** File description:
** info
*/

#include "server.h"

user_input_t *info_parser(char **user_input, UNUSED client_server_t *client)
{
    user_input_t *user_input_parsed = init_user_input_structure();

    if (user_input == NULL || user_input_parsed == NULL ||
    user_input[0] == NULL || user_input[1] != NULL) {
        free_user_input(user_input_parsed);
        return NULL;
    }
    user_input_parsed->command = strdup(user_input[0]);
    return user_input_parsed;
}
