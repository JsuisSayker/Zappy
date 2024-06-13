/*
** EPITECH PROJECT, 2024
** server_functions
** File description:
** server_functions
*/

#include <zappy_server.h>

static const struct command_s COMMAND_FUNCS[] = {
    {"/help", &server_command_help},
    {"/quit", &server_command_quit},
    {"/map", &server_command_map},
    {"/clients", &server_command_clients},
    {"/clear", &server_command_clear},
    {"/tile", &server_command_tile},
    {"/tp", &server_command_tp},
    {"/setTile", &server_command_set_tile},
    {"/setInventory", &server_command_set_inventory},
    {"/setFreq", &server_command_set_freq},
    {"/setLevel", &server_command_set_level},
    {"/send_guis", &server_command_send_guis},
    {"/send_guis", &server_command_kill},
    {"NULL", NULL}
};

int handle_server_command(zappy_server_t *zappy_server, char *command)
{
    if (command == NULL)
        return ERROR;
    for (int i = 0; COMMAND_FUNCS[i].func != NULL; i += 1) {
        if (strncmp(command, COMMAND_FUNCS[i].command,
            strlen(COMMAND_FUNCS[i].command)) == 0) {
            COMMAND_FUNCS[i].func(zappy_server,
                &command[strlen(COMMAND_FUNCS[i].command)]);
            return OK;
        }
    }
    return ERROR;
}
