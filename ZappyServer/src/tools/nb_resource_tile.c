/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** nb_resource_tile
*/

#include <zappy_server.h>

int nb_resource_tile(inventory_t *inventory)
{
    int nb = 0;

    nb += inventory->food;
    nb += inventory->linemate;
    nb += inventory->deraumere;
    nb += inventory->sibur;
    nb += inventory->mendiane;
    nb += inventory->phiras;
    nb += inventory->thystame;
    return nb;
}
