/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** error_display
*/

#include <zappy_server.h>

void error_command_argument(char *command, int nb_argument, int nb_expected)
{
    printf("[%s] : Invalid number of args, expected %d but got %d.\n", command,
        nb_expected, nb_argument);
}
