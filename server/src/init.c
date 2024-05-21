/*
** EPITECH PROJECT, 2024
** My_teams
** File description:
** init
*/

#include "server.h"

user_input_t *init_user_input_structure(void)
{
    user_input_t *user_input = malloc(sizeof(user_input_t));

    if (user_input == NULL)
        return NULL;
    user_input->command = NULL;
    user_input->params = calloc(sizeof(param_t), 1);
    if (user_input->params == NULL)
        return NULL;
    return user_input;
}
