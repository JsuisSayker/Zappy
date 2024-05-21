/*
** EPITECH PROJECT, 2024
** My_teams
** File description:
** create
*/

#include "server.h"

static user_input_t *parse_team(user_input_t *user_input_parsed,
    char **user_input)
{
    if (user_input[1] != NULL && user_input[2] != NULL &&
        user_input[3] == NULL) {
        user_input_parsed->params->team_name = strdup(user_input[1]);
        user_input_parsed->params->team_description = strdup(
            user_input[2]);
        return user_input_parsed;
    } else {
        free_user_input(user_input_parsed);
        return NULL;
    }
}

static user_input_t *parse_channel(user_input_t *user_input_parsed,
    char **user_input)
{
    if (user_input[1] != NULL && user_input[2] != NULL &&
        user_input[3] == NULL) {
        user_input_parsed->params->channel_name = strdup(user_input[1]);
        user_input_parsed->params->channel_description = strdup(
            user_input[2]);
        return user_input_parsed;
    } else {
        free_user_input(user_input_parsed);
        return NULL;
    }
}

static user_input_t *parse_thread(user_input_t *user_input_parsed,
    char **user_input)
{
    if (user_input[1] != NULL && user_input[2] != NULL &&
        user_input[3] == NULL) {
        user_input_parsed->params->thread_title = strdup(user_input[1]);
        user_input_parsed->params->thread_body = strdup(user_input[2]);
        return user_input_parsed;
    } else {
        free_user_input(user_input_parsed);
        return NULL;
    }
}

static user_input_t *parse_comment(user_input_t *user_input_parsed,
    char **user_input)
{
    if (user_input[1] != NULL && user_input[2] == NULL) {
        user_input_parsed->params->comment_body = strdup(user_input[1]);
        return user_input_parsed;
    } else {
        free_user_input(user_input_parsed);
        return NULL;
    }
}

user_input_t *create_parser(char **user_input, client_server_t *client)
{
    user_input_t *user_input_parsed = init_user_input_structure();

    if (user_input == NULL || user_input_parsed == NULL ||
    user_input[0] == NULL) {
        free_user_input(user_input_parsed);
        return NULL;
    }
    user_input_parsed->command = strdup(user_input[0]);
    if (client->context.team == NULL)
        return parse_team(user_input_parsed, user_input);
    if (client->context.channel == NULL)
        return parse_channel(user_input_parsed, user_input);
    if (client->context.thread == NULL)
        return parse_thread(user_input_parsed, user_input);
    else
        return parse_comment(user_input_parsed, user_input);
    return user_input_parsed;
}
