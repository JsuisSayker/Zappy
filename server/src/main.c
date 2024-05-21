/*
** EPITECH PROJECT, 2023
** main
** File description:
** main
*/

#include <unistd.h>
#include <string.h>
#include "server.h"

int main(int ac, char *const *const av)
{
    if (!av)
        return KO;
    if (ac != 2) {
        write(1, "bad usage of ./myteams_server\n", 31);
        display_help();
        return KO;
    }
    if (strcmp(av[1], "-help") == 0)
        return display_help();
    if (atoi(av[1]) <= 0) {
        write(1, "Port must be a positive number\n", 31);
        return KO;
    }
    return launch_server(av);
}
