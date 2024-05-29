/*
** EPITECH PROJECT, 2024
** server_functions
** File description:
** server_functions
*/

#include <zappy_server.h>

static const struct command_s COMMAND_FUNCS[] = {
    {"msz", &gui_command_msz},
    {"bct", &gui_command_bct},
    {"mct", &gui_command_mct},
    {"tna", &gui_command_tna},
    {"ppo", &gui_command_ppo},
    {"plv", &gui_command_plv},
    {"pin", &gui_command_pin},
    {"sgt", &gui_command_sgt},
    {"sst", &gui_command_sst},
    {"NULL", NULL}
};

int handle_gui_command(zappy_server_t *zappy_server, char *command)
{
    for (int i = 0; COMMAND_FUNCS[i].func != NULL; i += 1) {
        if (strncmp(command, COMMAND_FUNCS[i].command,
            strlen(COMMAND_FUNCS[i].command)) == 0) {
            COMMAND_FUNCS[i].func(zappy_server,
                &command[strlen(COMMAND_FUNCS[i].command)]);
            return OK;
        }
    }
    gui_command_suc(zappy_server, command);
    return ERROR;
}
