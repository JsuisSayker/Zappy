/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ai_command_incantation
*/

#include <zappy_server.h>

static int get_players(zappy_server_t *zappy, int x, int y)
{
    int nb_players = 0;

    for (int i = 0; i < FD_SETSIZE; i ++) {
        if (zappy->clients[i].pos.x == x
        && zappy->clients[i].pos.y == y)
            nb_players += 1;
    }
    return nb_players;
}

static bool check_resources(inventory_t *inventory, int lvl)
{
    inventory_t req[] = {
        {0, 1, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0, 0, 0},
        {0, 2, 1, 1, 0, 0, 0},
        {0, 1, 1, 2, 0, 0, 0},
        {0, 1, 2, 1, 3, 0, 0},
        {0, 1, 2, 3, 0, 1, 0},
        {0, 2, 2, 2, 2, 2, 1}
    };

    return inventory->linemate >= req[lvl - 1].linemate &&
    inventory->deraumere >= req[lvl - 1].deraumere &&
    inventory->sibur >= req[lvl - 1].sibur &&
    inventory->mendiane >= req[lvl - 1].mendiane &&
    inventory->phiras >= req[lvl - 1].phiras &&
    inventory->thystame >= req[lvl - 1].thystame;
}

static int remove_ressource(inventory_t *map, int lvl)
{
    inventory_t req[] = {
        {0, 1, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0, 0, 0},
        {0, 2, 1, 1, 0, 0, 0},
        {0, 1, 1, 2, 0, 0, 0},
        {0, 1, 2, 1, 3, 0, 0},
        {0, 1, 2, 3, 0, 1, 0},
        {0, 2, 2, 2, 2, 2, 1}
    };

    if (map == NULL)
        return ERROR;
    map->linemate -= req[lvl - 1].linemate;
    map->deraumere -= req[lvl - 1].deraumere;
    map->sibur -= req[lvl - 1].sibur;
    map->mendiane -= req[lvl - 1].mendiane;
    map->phiras -= req[lvl - 1].phiras;
    map->thystame -= req[lvl - 1].thystame;
    return OK;
}

static bool check_player(int nb_players, int level)
{
    const int req[] = {1, 2, 2, 4, 4, 6, 6};

    return nb_players >= req[level - 1];
}

static bool check_incantation(zappy_server_t *zappy, client_t *client)
{
    int nb_players = 0;
    inventory_t *map;

    if (client == NULL || zappy == NULL)
        return false;
    map = &zappy->map_tile[client->pos.y][client->pos.x].inventory;
    nb_players = get_players(zappy, client->pos.x, client->pos.y);
    if (check_resources(map, client->level)
        && check_player(nb_players, client->level)) {
        if (client->incantation == false) {
            dprintf(zappy->actual_sockfd, "Elevation underway\n");
            send_pic_command_to_all_gui(zappy, client);
            client->incantation = true;
        }
            return true;
    }
    return false;
}

static int complet_incantation(zappy_server_t *zappy, client_t *client,
    int lvl)
{
    inventory_t *map;

    if (client == NULL || zappy == NULL)
        return ERROR;
    map = &zappy->map_tile[client->pos.x][client->pos.y].inventory;
    if (remove_ressource(map, lvl) == ERROR)
        return ERROR;
    client->level += 1;
    client->incantation = false;
    send_pie_command_to_all_gui(zappy, client);
    send_plv_command_to_all_gui(zappy, client);
    dprintf(zappy->actual_sockfd, "Current level: %d\n", client->level);
    return OK;
}

int ai_command_incantation(zappy_server_t *zappy, client_t *client, char *cmd)
{
    if (client == NULL || zappy == NULL || cmd == NULL)
        return ERROR;
    if (check_incantation(zappy, client) == false){
        dprintf(zappy->actual_sockfd, "ko bit\n");
        free_string(&client->command.execution);
        return OK;
    }
    if (cast_action(zappy, client, 300, cmd) == ERROR)
        return ERROR;
    if (check_action(zappy, client) == false)
        return OK;
    if (check_incantation(zappy, client) == false){
        dprintf(zappy->actual_sockfd, "ko\n");
        return OK;
    }
    if (complet_incantation(zappy, client, client->level) == ERROR)
        return ERROR;
    return OK;
}
