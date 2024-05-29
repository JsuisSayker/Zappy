/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** test_splitter
*/

#include <criterion/criterion.h>
#include <zappy_server.h>

Test(handle_server_command, test_handle_server_command_no_a_command)
{
    zappy_server_t zappy_server;
    char *command = "no_a_command";
    cr_assert_eq(handle_server_command(&zappy_server, command), ERROR);
}

Test(handle_server_command, test_handle_server_null)
{
    zappy_server_t zappy_server;
    cr_assert_eq(handle_server_command(&zappy_server, NULL), ERROR);
}