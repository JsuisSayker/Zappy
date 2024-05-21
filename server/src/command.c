/*
** EPITECH PROJECT, 2024
** My_teams
** File description:
** comman
*/

#include "server.h"

static const struct parse_command_s PARSE_COMMAND[] = {
    {"/login", &login_parser},
    {"/logout", &logout_parser},
    {"/users", &users_parser},
    {"/user", &user_parser},
    {"/send", &send_parser},
    {"/messages", &messages_parser},
    {"/subscribe", &subscribe_parser},
    {"/subscribed", &subscribed_parser},
    {"/unsubscribe", &unsubscribe_parser},
    {"/use", &use_parser},
    {"/create", &create_parser},
    {"/list", &list_parser},
    {"/info", &info_parser},
    {"/help", &help_parser},
    {NULL, NULL}
};

static int parse_user_input_sub(char *user_input, char ***parsed_input,
    char *tmp, int i)
{
    if (user_input[i] == '\"') {
        i++;
        tmp = strdup(user_input + i);
        *parsed_input = tab_append_str_at_end(*parsed_input,
            strtok(tmp, "\""));
        for (; user_input[i] != '\0' && user_input[i] != '\"'; i++);
        if (tmp != NULL)
            free(tmp);
        return i;
    }
    free_tab(*parsed_input);
    return ERROR;
}

static char **loop_parse_user_input(char **parsed_input, char *user_input,
    char *tmp, int i)
{
    parsed_input = tab_append_str_at_end(parsed_input, strtok(tmp, " \t"));
    if (tmp != NULL)
        free(tmp);
    for (; user_input[i] != '\0' && user_input[i] != ' ' &&
    user_input[i] != '\t'; i++);
    for (; user_input[i] != '\0'; i++) {
        if (user_input[i] == ' ' || user_input[i] == '\t')
            continue;
        i = parse_user_input_sub(user_input, &parsed_input, tmp, i);
        if (i == ERROR)
            return NULL;
    }
    return parsed_input;
}

static char **parse_user_input(char *user_input)
{
    int i = 0;
    char *tmp = strdup(user_input);
    char **parsed_input = NULL;

    if (user_input == NULL || user_input[0] != '/') {
        free(tmp);
        return NULL;
    }
    return loop_parse_user_input(parsed_input, user_input, tmp, i);
}

static int free_old_user_input(char **user_input, client_server_t *client)
{
    if (client == NULL)
        return ERROR;
    free_tab(user_input);
    if (client->user_input != NULL)
        free(client->user_input);
    client->user_input = NULL;
    return OK;
}

static int parse_and_launch_command_sub(char **user_input,
    client_server_t *client, server_data_t *server_data)
{
    for (int i = 0; PARSE_COMMAND[i].command != NULL; i++) {
        if (strcmp(PARSE_COMMAND[i].command, user_input[0]) == 0)
            client->command = PARSE_COMMAND[i].func(user_input, client);
    }
    if (client->command == NULL)
        write(client->socket, "214|bad command, type /help\a\n", 30);
    else {
        if (find_command(server_data, client) == ERROR)
            return ERROR;
    }
    free_old_user_input(user_input, client);
    return OK;
}

static int parse_and_launch_command(server_data_t *server_data,
    client_server_t *client)
{
    char **user_input = NULL;

    client->user_input[strlen(client->user_input) - 2] = '\0';
    user_input = parse_user_input(client->user_input);
    if (user_input == NULL) {
        write(client->socket, "214|bad command, type /help\a\n", 30);
        return free_old_user_input(user_input, client);
    }
    return parse_and_launch_command_sub(user_input, client, server_data);
}

int check_command(server_data_t *server_data, client_server_t *client)
{
    if (client->user_input[strlen(client->user_input) - 1] ==
    '\n' && client->user_input[strlen(client->user_input) - 2]
    == '\a') {
        if (parse_and_launch_command(server_data, client) == ERROR)
            return ERROR;
        free_user_input(client->command);
        client->command = NULL;
    }
}
