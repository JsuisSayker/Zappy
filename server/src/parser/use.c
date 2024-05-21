/*
** EPITECH PROJECT, 2024
** My_teams
** File description:
** use
*/

#include "server.h"

user_input_t *use_parser(char **user_input, UNUSED client_server_t *client)
{
    user_input_t *user_input_parsed = init_user_input_structure();

    if (user_input == NULL || user_input_parsed == NULL ||
    user_input[0] == NULL) {
        free_user_input(user_input_parsed);
        return NULL;
    }
    user_input_parsed->command = strdup(user_input[0]);
    if (user_input[1] != NULL)
        user_input_parsed->params->team_uuid = strdup(user_input[1]);
    if (user_input[1] != NULL && user_input[2] != NULL)
        user_input_parsed->params->channel_uuid = strdup(user_input[2]);
    if (user_input[1] != NULL && user_input[2] != NULL &&
    user_input[3] != NULL)
        user_input_parsed->params->thread_uuid = strdup(user_input[3]);
    return user_input_parsed;
}
