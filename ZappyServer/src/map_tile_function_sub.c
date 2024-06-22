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
        tile.x, tile.y, tile.inventory.food, tile.inventory.linemate,
        tile.inventory.deraumere, tile.inventory.sibur,
        tile.inventory.mendiane, tile.inventory.phiras,
        tile.inventory.thystame);
}

void display_gui_tile(map_tile_t *tile, int socket)
{
    dprintf(socket, "bct %d %d %d %d %d %d %d %d %d\n", tile->x, tile->y,
        tile->inventory.food, tile->inventory.linemate,
        tile->inventory.deraumere, tile->inventory.sibur,
        tile->inventory.mendiane, tile->inventory.phiras,
        tile->inventory.thystame);
}

void display_map_tile(map_tile_t **map_tile)
{
    for (int i = 0; map_tile[i] != NULL; i += 1) {
        for (int j = 0; map_tile[i][j].x != -1; j += 1) {
            display_tile(map_tile[i][j]);
            printf("<======>\n");
        }
    }
    printf("<==============>\n");
}

int get_len_map_tile(map_tile_t **map_tile)
{
    int len = 0;

    for (len = 0; map_tile[len] != NULL; len += 1) {
        len += 1;
    }
    return len;
}

int get_len_line_map_tile(map_tile_t **map_tile)
{
    int len = 0;

    for (len = 0; map_tile[0][len].x != -1; len += 1) {
        len += 1;
    }
    return len;
}
