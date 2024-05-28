/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** test_splitter
*/

#include <criterion/criterion.h>
#include <zappy_server.h>

Test(server_command_set_freq, test_server_command_set_freq)
{
    zappy_server_t zappy_server;
    zappy_server.args = calloc(sizeof(args_config_t), 1);
    char *command = "/setFreq 100";
    cr_assert_eq(handle_server_command(&zappy_server, command), OK);
    free(zappy_server.args);
}

Test(server_command_set_freq, test_server_command_set_freq_null)
{
    zappy_server_t zappy_server;
    char *command = "/setFreq";
    cr_assert_eq(handle_server_command(&zappy_server, command), OK);
}