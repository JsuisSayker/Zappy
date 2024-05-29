/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** test_splitter
*/

#include <criterion/criterion.h>
#include <zappy_server.h>

Test(server_command_map, test_server_command_map)
{
    zappy_server_t zappy_server;
    zappy_server.map_tile = setup_map_tile(10, 10);
    char *command = "/map";
    cr_assert_eq(handle_server_command(&zappy_server, command), OK);
    free_map_tile(zappy_server.map_tile);
}