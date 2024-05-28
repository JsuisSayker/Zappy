/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** test_splitter
*/

#include <criterion/criterion.h>
#include <zappy_server.h>

// Test(server_command_tile, test_server_command_tile)
// {
//     zappy_server_t zappy_server;
//     zappy_server.map_tile = setup_map_tile(10, 10);
//     char *command = "/tile 1 1";
//     cr_assert_eq(handle_server_command(&zappy_server, command), OK);
//     free_map_tile(zappy_server.map_tile);
// }

Test(server_command_tile, test_server_command_tile_null)
{
    zappy_server_t zappy_server;
    char *command = "/tile";
    cr_assert_eq(handle_server_command(&zappy_server, command), OK);
}