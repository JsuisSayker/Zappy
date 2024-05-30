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
        tile.inventory.food, tile.inventory.linemate, tile.inventory.deraumere,
        tile.inventory.sibur, tile.inventory.mendiane, tile.inventory.phiras,
        tile.inventory.thystame);
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

map_tile_t **copy_map_tile(map_tile_t **source)
{
    int len_x = get_len_line_map_tile(source);
    int len_y = get_len_map_tile(source);
    map_tile_t **destination = calloc(sizeof(map_tile_t *), len_y + 1);

    for (int i = 0; source[i] != NULL; i += 1) {
        destination[i] = calloc(sizeof(map_tile_t), len_x + 1);
        for (int j = 0; source[i][j].x != -1; j += 1) {
            destination[i][j] = source[i][j];
        }
        destination[i][len_x].x = -1;
        destination[i][len_x].y = -1;
    }
    destination[len_y] = NULL;
    return destination;
}


void refill_inventory(inventory_t *destination, inventory_t *source)
{
    if (destination->food < source->food)
        destination->food = source->food;
    if (destination->linemate < source->linemate)
        destination->linemate = source->linemate;
    if (destination->deraumere < source->deraumere)
        destination->deraumere = source->deraumere;
    if (destination->sibur < source->sibur)
        destination->sibur = source->sibur;
    if (destination->mendiane < source->mendiane)
        destination->mendiane = source->mendiane;
    if (destination->phiras < source->phiras)
        destination->phiras = source->phiras;
    if (destination->thystame < source->thystame)
        destination->thystame = source->thystame;
}

void refill_map_tile(map_tile_t **destination, map_tile_t **source)
{
    for (int i = 0; source[i] != NULL; i += 1) {
        for (int j = 0; source[i][j].x != -1; j += 1) {
            refill_inventory(&destination[i][j].inventory,
                &source[i][j].inventory);
        }
    }
}