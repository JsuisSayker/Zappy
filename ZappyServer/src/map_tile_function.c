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
    map_tile[y][x].food = 0;
    map_tile[y][x].linemate = 0;
    map_tile[y][x].deraumere = 0;
    map_tile[y][x].sibur = 0;
    map_tile[y][x].mendiane = 0;
    map_tile[y][x].phiras = 0;
    map_tile[y][x].thystame = 0;
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
        tile_map_tile->food += 1;
    if (strcmp(resourse, "linemate") == 0)
        tile_map_tile->linemate += 1;
    if (strcmp(resourse, "deraumere") == 0)
        tile_map_tile->deraumere += 1;
    if (strcmp(resourse, "sibur") == 0)
        tile_map_tile->sibur += 1;
    if (strcmp(resourse, "mendiane") == 0)
        tile_map_tile->mendiane += 1;
    if (strcmp(resourse, "phiras") == 0)
        tile_map_tile->phiras += 1;
    if (strcmp(resourse, "thystame") == 0)
        tile_map_tile->thystame += 1;
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

void display_map_tile(map_tile_t **map_tile)
{
    int i = 0;
    int j = 0;

    for (i = 0; map_tile[i] != NULL; i += 1) {
        for (j = 0; map_tile[i][j].x != -1; j += 1) {
            printf("Tile: [%d][%d]\nFood: %d\nLinemate: %d\nDeraumere: %d\n"
                    "Sibur: %d\nMendiane: %d\nPhiras: %d\nThystame: %d\n"
                    "<======>\n",
                map_tile[i][j].x, map_tile[i][j].y, map_tile[i][j].food,
                map_tile[i][j].linemate, map_tile[i][j].deraumere,
                map_tile[i][j].sibur, map_tile[i][j].mendiane,
                map_tile[i][j].phiras, map_tile[i][j].thystame);
        }
    }
    printf("<==============>\n");
}
