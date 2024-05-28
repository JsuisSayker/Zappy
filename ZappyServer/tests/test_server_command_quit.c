/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** test_splitter
*/

#include <criterion/criterion.h>
#include <zappy_server.h>

Test(server_command_quit, test_server_command_quit)
{
    zappy_server_t zappy_server;
    char *command = "/quit";
    cr_assert_eq(handle_server_command(&zappy_server, command), OK);
}