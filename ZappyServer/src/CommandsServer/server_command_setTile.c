/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

void server_command_setTile(zappy_server_t *zappy, char *command)
{
    char *resource = NULL;
    int resource_quantity = 0;
    int x = 0;
    int y = 0;
    char **command_array = NULL;

    if (command == NULL || command[0] != ' ')
        return;
    command_array = splitter(command, " ");
    if (get_len_char_tab(command_array) != 4)
        return;
    resource = command_array[0];
    resource_quantity = atoi(command_array[1]);
    x = atoi(command_array[2]);
    y = atoi(command_array[3]);
    if (resource_quantity > 0 && x >= 0 && x < zappy->args->width && y >= 0 &&
        y < zappy->args->height) {
        set_inventory_resource_quantite(&zappy->map_tile[y][x].inventory,
            resource, resource_quantity);
        printf("tp done\n");
    } else {
        printf("tp failed\n");
    }
    
}
