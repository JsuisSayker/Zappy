/*
** EPITECH PROJECT, 2024
** My_teams
** File description:
** display_help
*/

#include <unistd.h>
#include "macro.h"

int display_help(void)
{
    if (write(1, "USAGE: ./myteams_server port\n\n\
    port is the port number on which the server socket listens.\n", 95) == -1)
        return KO;
    return OK;
}
