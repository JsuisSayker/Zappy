/*
** EPITECH PROJECT, 2024
** My_teams
** File description:
** user
*/

#include "server.h"

user_input_t *user_parser(char **user_input, UNUSED client_server_t *client)
{
    user_input_t *user_input_parsed = init_user_input_structure();

    if (user_input == NULL || user_input_parsed == NULL ||
    user_input[0] == NULL || user_input[1] == NULL || user_input[2] != NULL) {
        free_user_input(user_input_parsed);
        return NULL;
    }
    user_input_parsed->command = strdup(user_input[0]);
    user_input_parsed->params->user_uuid = strdup(user_input[1]);
    return user_input_parsed;
}
