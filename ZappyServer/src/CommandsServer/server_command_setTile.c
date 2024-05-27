/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

static void set_resource_quantite(inventory_t *tile_inventory, char *resource, int resource_quantity)
{
    if (strcmp(resource, "food") == 0)
        tile_inventory->food = resource_quantity;
    if (strcmp(resource, "linemate") == 0)
        tile_inventory->linemate = resource_quantity;
    if (strcmp(resource, "deraumere") == 0)
        tile_inventory->deraumere = resource_quantity;
    if (strcmp(resource, "sibur") == 0)
        tile_inventory->sibur = resource_quantity;
    if (strcmp(resource, "mendiane") == 0)
        tile_inventory->mendiane = resource_quantity;
    if (strcmp(resource, "phiras") == 0)
        tile_inventory->phiras = resource_quantity;
    if (strcmp(resource, "thystame") == 0)
        tile_inventory->thystame = resource_quantity;
}

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
        set_resource_quantite(&zappy->map_tile[y][x].inventory, resource,
            resource_quantity);
        printf("tp done\n");
    } else {
        printf("tp failed\n");
    }
    
}
