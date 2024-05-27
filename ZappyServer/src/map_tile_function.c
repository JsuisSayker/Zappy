/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** generate_map
*/

#include <zappy_server.h>

static void init_tile_value(map_tile_t **map_tile, int x, int y)
{
    map_tile[y][x].x = x;
    map_tile[y][x].y = y;
    map_tile[y][x].inventory.food = 0;
    map_tile[y][x].inventory.linemate = 0;
    map_tile[y][x].inventory.deraumere = 0;
    map_tile[y][x].inventory.sibur = 0;
    map_tile[y][x].inventory.mendiane = 0;
    map_tile[y][x].inventory.phiras = 0;
    map_tile[y][x].inventory.thystame = 0;
}

map_tile_t **generate_map_tile(int x, int y)
{
    int i = 0;
    int j = 0;
    map_tile_t **map_tile = calloc(sizeof(map_tile_t *), (y + 1));

    for (i = 0; i < y; i += 1) {
        map_tile[i] = calloc(sizeof(map_tile_t), (x + 1));
        for (j = 0; j < x; j += 1) {
            init_tile_value(map_tile, j, i);
        }
        map_tile[i][j].x = -1;
        map_tile[i][j].y = -1;
    }
    map_tile[i] = NULL;
    return map_tile;
}

void free_map_tile(map_tile_t **map_tile)
{
    for (int i = 0; map_tile[i] != NULL; i += 1) {
        free(map_tile[i]);
    }
    free(map_tile);
}

static void add_resource_to_tile(map_tile_t *tile_map_tile, char *resourse)
{
    if (strcmp(resourse, "food") == 0)
        tile_map_tile->inventory.food += 1;
    if (strcmp(resourse, "linemate") == 0)
        tile_map_tile->inventory.linemate += 1;
    if (strcmp(resourse, "deraumere") == 0)
        tile_map_tile->inventory.deraumere += 1;
    if (strcmp(resourse, "sibur") == 0)
        tile_map_tile->inventory.sibur += 1;
    if (strcmp(resourse, "mendiane") == 0)
        tile_map_tile->inventory.mendiane += 1;
    if (strcmp(resourse, "phiras") == 0)
        tile_map_tile->inventory.phiras += 1;
    if (strcmp(resourse, "thystame") == 0)
        tile_map_tile->inventory.thystame += 1;
}

static void add_all_resource_to_tile(map_tile_t **map_tile, char_tab_t **tmp,
    int *index_x, int **possible_coordinate)
{
    if (tmp == NULL)
        return;
    for (int j = 0; j < index_x[1] && (*tmp) != NULL; j += 1) {
        add_resource_to_tile(&map_tile[possible_coordinate[(index_x[0])][1]]
                    [possible_coordinate[(index_x[0])][0]], (*tmp)->str);
        (*tmp) = TAILQ_NEXT((*tmp), next);
        index_x[0] += 1;
    }
}

void put_resource_on_map_tile(
    map_tile_t **map_tile, struct char_tab_head *head, int x, int y)
{
    int index_x[2] = {0, x};
    int **possible_coordinate = NULL;
    char_tab_t *tmp = TAILQ_FIRST(head);

    while (tmp != NULL) {
        index_x[0] = 0;
        possible_coordinate = generate_int_array(x, y);
        shuffle_int_array(possible_coordinate, x * y);
        for (int i = 0; i < y && tmp != NULL; i += 1) {
            add_all_resource_to_tile(map_tile, &tmp, index_x,
                possible_coordinate);
        }
        free_int_array(possible_coordinate);
    }
    return;
}

map_tile_t **setup_map_tile(int x, int y)
{
    struct char_tab_head *resourse_list = generate_ressourse_list(x, y);
    map_tile_t **map_tile = generate_map_tile(x, y);

    put_resource_on_map_tile(map_tile, resourse_list, x, y);
    free_char_tab_list(resourse_list);
    free(resourse_list);
    return map_tile;
}
