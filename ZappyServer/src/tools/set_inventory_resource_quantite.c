/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** set_inventory_resource_quantite
*/

#include <zappy_server.h>

void set_inventory_resource_quantite(inventory_t *tile_inventory,
    char *resource, int resource_quantity)
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
