/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ai_command_incantation
*/

#include <zappy_server.h>

static int get_players(zappy_server_t *zappy, int x, int y, int lvl)
{
    int nb_players = 0;

    for (int i = 3; i < zappy->nb_connected_clients; i ++) {
        if (zappy->clients[i].type == AI && zappy->clients[i].pos.x == x
        && zappy->clients[i].pos.y == y && zappy->clients[i].level == lvl)
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

static int remove_ressource(inventory_t *tile_inventory, int lvl)
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

    if (check_resources(tile_inventory, lvl) == false)
        return ERROR;
    tile_inventory->linemate -= req[lvl - 1].linemate;
    tile_inventory->deraumere -= req[lvl - 1].deraumere;
    tile_inventory->sibur -= req[lvl - 1].sibur;
    tile_inventory->mendiane -= req[lvl - 1].mendiane;
    tile_inventory->phiras -= req[lvl - 1].phiras;
    tile_inventory->thystame -= req[lvl - 1].thystame;
    return OK;
}

static bool check_player(int nb_players, int level)
{
    const int req[] = {1, 2, 2, 4, 4, 6, 6};

    return nb_players >= req[level - 1];
}

bool incantation_condition(zappy_server_t *zappy, client_t *client)
{
    int nb_players = 0;
    inventory_t *map;

    if (client == NULL || zappy == NULL)
        return false;
    map = &zappy->map_tile[client->pos.y][client->pos.x].inventory;
    nb_players = get_players(zappy, client->pos.x, client->pos.y,
        client->level);
    if (check_resources(map, client->level)
        && check_player(nb_players, client->level)) {
        if (client->incantation == false) {
            dprintf(zappy->actual_sockfd, "Elevation underway\n");
            send_pic_command_to_all_gui(zappy, client);
            client->incantation = true;
        }
        return true;
    }
    send_pie_command_to_all_gui(zappy, client, 0);
    return false;
}

static void add_level(zappy_server_t *zappy, client_t *client)
{
    team_t *team = NULL;

    client->level += 1;
    if (client->level == 8){
        TAILQ_FOREACH(team, &zappy->all_teams, next) {
            if (strcmp(team->name, client->team_name) == 0) {
                team->nb_clients_lvl_8 += 1;
                break;
            }
        }
    }
}

static int complet_incantation(zappy_server_t *zappy, client_t *client,
    int lvl)
{
    inventory_t *tile_inventory;

    if (client == NULL || zappy == NULL)
        return ERROR;
    tile_inventory = &zappy->map_tile[client->pos.x][client->pos.y].inventory;
    if (remove_ressource(tile_inventory, lvl) == ERROR)
        return ERROR;
    for (int i = 0; i < zappy->nb_connected_clients; i++) {
        if (zappy->clients[i].type == AI && zappy->clients[i].pos.x ==
        client->pos.x && zappy->clients[i].pos.y == client->pos.y
        && zappy->clients[i].level == lvl) {
            add_level(zappy, &zappy->clients[i]);
            zappy->clients[i].incantation = false;
            send_pie_command_to_all_gui(zappy, &zappy->clients[i], 1);
            send_plv_command_to_all_gui(zappy, &zappy->clients[i]);
            dprintf(i, "Current level: %d\n", zappy->clients[i].level);
            client->incantation = false;
        }
    }
    return OK;
}

int ai_command_incantation(zappy_server_t *zappy, client_t *client, char *cmd)
{
    if (client == NULL || zappy == NULL || cmd == NULL)
        return ERROR;
    if (complet_incantation(zappy, client, client->level) == ERROR){
        dprintf(zappy->actual_sockfd, "ko\n");
        send_pie_command_to_all_gui(zappy, client, 1);
        client->incantation = false;
    }
    return OK;
}
