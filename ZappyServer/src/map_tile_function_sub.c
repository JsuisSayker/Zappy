/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** generate_map
*/

#include <zappy_server.h>

void display_tile(map_tile_t tile)
{
    printf("Tile: [%d][%d]\nFood: %d\nLinemate: %d\nDeraumere: %d\n"
        "Sibur: %d\nMendiane: %d\nPhiras: %d\nThystame: %d\n",
        tile.x, tile.y, tile.inventory.food,
        tile.inventory.linemate, tile.inventory.deraumere,
        tile.inventory.sibur, tile.inventory.mendiane,
        tile.inventory.phiras, tile.inventory.thystame);
}

void display_gui_tile(map_tile_t tile, int socket)
{
    dprintf(socket, "bct %d %d %d %d %d %d %d %d %d\n", tile.x, tile.y,
        tile.inventory.food, tile.inventory.linemate, tile.inventory.deraumere, tile.inventory.sibur,
        tile.inventory.mendiane, tile.inventory.phiras, tile.inventory.thystame);
}

void display_map_tile(map_tile_t **map_tile)
{
    int i = 0;
    int j = 0;

    for (i = 0; map_tile[i] != NULL; i += 1) {
        for (j = 0; map_tile[i][j].x != -1; j += 1) {
            display_tile(map_tile[i][j]);
            printf("<======>\n");
        }
    }
    printf("<==============>\n");
}
