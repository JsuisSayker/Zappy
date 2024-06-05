/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** generate_map
*/

#include <zappy_server.h>

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

static bool refill_inventory_sub(map_tile_t *destination, inventory_t *source)
{
    bool change = false;

    if (destination->inventory.sibur < source->sibur) {
        destination->inventory.sibur = source->sibur;
        change = true;
    }
    if (destination->inventory.mendiane < source->mendiane) {
        destination->inventory.mendiane = source->mendiane;
        change = true;
    }
    if (destination->inventory.phiras < source->phiras) {
        destination->inventory.phiras = source->phiras;
        change = true;
    }
    if (destination->inventory.thystame < source->thystame) {
        destination->inventory.thystame = source->thystame;
        change = true;
    }
    return change;
}

static bool refill_inventory(map_tile_t *destination, inventory_t *source)
{
    bool change = false;

    if (destination->inventory.food < source->food) {
        destination->inventory.food = source->food;
        change = true;
    }
    if (destination->inventory.linemate < source->linemate) {
        destination->inventory.linemate = source->linemate;
        change = true;
    }
    if (destination->inventory.deraumere < source->deraumere) {
        destination->inventory.deraumere = source->deraumere;
        change = true;
    }
    if (refill_inventory_sub(destination, source))
        change = true;
    return change;
}

void refill_map_tile_ter(zappy_server_t *zappy_server,
    map_tile_t **destination, int i, int j)
{
    for (int k = 0; k < FD_SETSIZE; k++) {
        if (zappy_server->clients[k].type == GUI) {
            display_gui_tile(&destination[i][j], k);
            return;
        }
    }
}

void refill_map_tile_sub(zappy_server_t *zappy_server,
    map_tile_t **destination, map_tile_t **source, int i)
{
    for (int j = 0; source[i][j].x != -1; j += 1) {
        if (refill_inventory(&destination[i][j],
        &source[i][j].inventory)) {
            refill_map_tile_ter(zappy_server, destination, i, j);
        }
    }
}

void refill_map_tile(zappy_server_t *zappy_server, map_tile_t **destination,
    map_tile_t **source)
{
    for (int i = 0; source[i] != NULL; i += 1) {
        refill_map_tile_sub(zappy_server, destination, source, i);
    }
}
