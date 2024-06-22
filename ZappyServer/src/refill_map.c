/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** refill_map
*/

#include <zappy_server.h>

static void add_resource_1(zappy_server_t *zappy, bool *is_refill)
{
    int resource_to_add = (FOOD_DENSITY * zappy->args->width *
        zappy->args->height / 100) - zappy->all_resources[0];

    for (int i = 0; i < resource_to_add; i += 1) {
        zappy->map_tile[rand() % zappy->args->height]
            [rand() % zappy->args->width].inventory.food += 1;
        zappy->all_resources[0] += 1;
        (*is_refill) = true;
    }
    resource_to_add = (LINEMATE_DENSITY * zappy->args->width *
            zappy->args->height / 100) - zappy->all_resources[1];
    for (int i = 0; i < resource_to_add; i += 1) {
        zappy->map_tile[rand() % zappy->args->height]
            [rand() % zappy->args->width].inventory.linemate += 1;
        zappy->all_resources[1] += 1;
        (*is_refill) = true;
    }
}

static void add_resource_2(zappy_server_t *zappy, bool *is_refill)
{
    int resource_to_add = (DERAUMERE_DENSITY * zappy->args->width *
            zappy->args->height / 100) - zappy->all_resources[2];

    for (int i = 0; i < resource_to_add; i += 1) {
        zappy->map_tile[rand() % zappy->args->height]
            [rand() % zappy->args->width].inventory.deraumere += 1;
        zappy->all_resources[2] += 1;
        (*is_refill) = true;
    }
    resource_to_add = (SIBUR_DENSITY * zappy->args->width *
        zappy->args->height / 100) - zappy->all_resources[3];
    for (int i = 0; i < resource_to_add; i += 1) {
        zappy->map_tile[rand() % zappy->args->height]
            [rand() % zappy->args->width].inventory.sibur += 1;
        zappy->all_resources[3] += 1;
        (*is_refill) = true;
    }
}

static void add_resource_3(zappy_server_t *zappy, bool *is_refill)
{
    int resource_to_add = (MENDIANE_DENSITY * zappy->args->width *
        zappy->args->height / 100) - zappy->all_resources[4];

    for (int i = 0; i < resource_to_add; i += 1) {
        zappy->map_tile[rand() % zappy->args->height]
            [rand() % zappy->args->width].inventory.mendiane += 1;
        zappy->all_resources[4] += 1;
        (*is_refill) = true;
    }
    resource_to_add = (PHIRAS_DENSITY * zappy->args->width *
        zappy->args->height / 100) - zappy->all_resources[5];
    for (int i = 0; i < resource_to_add; i += 1) {
        zappy->map_tile[rand() % zappy->args->height]
            [rand() % zappy->args->width].inventory.phiras += 1;
        zappy->all_resources[5] += 1;
        (*is_refill) = true;
    }
}

static void add_resource_4(zappy_server_t *zappy, bool *is_refill)
{
    int resource_to_add = (THYSTAME_DENSITY * zappy->args->width *
            zappy->args->height / 100) - zappy->all_resources[6];

    for (int i = 0; i < resource_to_add; i += 1) {
        zappy->map_tile[rand() % zappy->args->height]
            [rand() % zappy->args->width].inventory.phiras += 1;
        zappy->all_resources[6] += 1;
        (*is_refill) = true;
    }
}

void refill_map(zappy_server_t *zappy)
{
    struct timeval tv;
    double cast_time = 20 / (double)zappy->args->freq;
    double elapsed = 0;
    bool is_refill = false;

    gettimeofday(&tv, NULL);
    elapsed = (tv.tv_sec + tv.tv_usec / 1000000.0) - zappy->time_refill_map;
    if (cast_time < elapsed) {
        add_resource_1(zappy, &is_refill);
        add_resource_2(zappy, &is_refill);
        add_resource_3(zappy, &is_refill);
        add_resource_4(zappy, &is_refill);
        if (is_refill)
            send_mct_command_to_all_gui(zappy);
        zappy->time_refill_map = time(NULL);
    }
}