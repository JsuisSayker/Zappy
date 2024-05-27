/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

static void server_command_set_tile_sub(zappy_server_t *zappy,
    char **command_array)
{
    char *resource = command_array[0];
    int resource_quantity = atoi(command_array[1]);
    int x = atoi(command_array[2]);
    int y = atoi(command_array[3]);

    if (resource_quantity > 0 && x >= 0 && x < zappy->args->width && y >= 0 &&
        y < zappy->args->height) {
        set_inventory_resource_quantite(&zappy->map_tile[y][x].inventory,
            resource, resource_quantity);
        printf("tp done\n");
    }
}

void server_command_set_tile(zappy_server_t *zappy, char *command)
{
    char **command_array = NULL;

    if (command == NULL || command[0] != ' ')
        return;
    command_array = splitter(command, " ");
    if (get_len_char_tab(command_array) != 4)
        return;
    server_command_set_tile_sub(zappy, command_array);
    free_array(command_array);
}
