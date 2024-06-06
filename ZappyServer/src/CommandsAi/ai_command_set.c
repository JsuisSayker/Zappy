/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ai_command_set
*/

#include <zappy_server.h>

static int get_item(char *cmd, char **item)
{
    char **tmp = NULL;

    if (cmd == NULL)
        return ERROR;
    tmp = splitter(cmd, " ");
    if (tmp == NULL)
        return ERROR;
    if (tmp[1] == NULL)
        return ERROR;
    *item = strdup(tmp[1]);
    for (int i = 0; tmp[i] != NULL; i += 1)
        free(tmp[i]);
    free(tmp);
    return OK;
}

static int drop_item(int *map_item, int *item)
{
    if (map_item == NULL || item == NULL)
        return ERROR;
    if (*item == 0)
        return ERROR;
    *map_item = *item;
    *item = 0;
    return OK;
}

static int find_item_sub(zappy_server_t *zappy, client_t *client, char *item)
{
    if (zappy == NULL || client == NULL || item == NULL)
        return ERROR;
    if (strncmp(item, "sibur", strlen("sibur\0")) == 0)
        return drop_item(
            &zappy->map_tile[client->pos.y][client->pos.x].inventory.sibur,
            &client->inventory.sibur);
    if (strncmp(item, "mendiane", strlen("mendiane\0")) == 0)
        return drop_item(
            &zappy->map_tile[client->pos.y][client->pos.x].inventory.mendiane,
            &client->inventory.mendiane);
    if (strncmp(item, "phiras", strlen("phiras\0")) == 0)
        return drop_item(
            &zappy->map_tile[client->pos.y][client->pos.x].inventory.phiras,
            &client->inventory.phiras);
    if (strncmp(item, "thystame", strlen("thystame\0")) == 0)
        return drop_item(
            &zappy->map_tile[client->pos.y][client->pos.x].inventory.thystame,
            &client->inventory.thystame);
    return ERROR;
}

static int find_item(zappy_server_t *zappy, client_t *client, char *item)
{
    if (zappy == NULL || client == NULL || item == NULL)
        return ERROR;
    if (strncmp(item, "food", strlen("food\0")) == 0)
        return drop_item(
            &zappy->map_tile[client->pos.y][client->pos.x].inventory.food,
            &client->inventory.food);
    if (strncmp(item, "linemate", strlen("linemate\0")) == 0)
        return drop_item(
            &zappy->map_tile[client->pos.y][client->pos.x].inventory.linemate,
            &client->inventory.linemate);
    if (strncmp(item, "deraumere", strlen("deraumere\0")) == 0)
        return drop_item(
            &zappy->map_tile[client->pos.y][client->pos.x].inventory.deraumere,
            &client->inventory.deraumere);
    if (find_item_sub(zappy, client, item) == ERROR)
        return ERROR;
    return ERROR;
}

static int set_command(zappy_server_t *zappy, client_t *client, char *cmd)
{
    char *item = NULL;

    if (client == NULL || zappy == NULL || cmd == NULL)
        return ERROR;
    if (get_item(cmd, &item) != OK)
        return ERROR;
    if (find_item(zappy, client, item) == ERROR){
        dprintf(zappy->actual_sockfd, "ko\n");
        free(item);
        return OK;
    }
    send_pin_command_to_all_gui(zappy, &zappy->clients[zappy->actual_sockfd]);
    dprintf(zappy->actual_sockfd, "ok\n");
    free(item);
    return OK;
}

int ai_command_set(zappy_server_t *zappy, client_t *client, char *cmd)
{
    if (client == NULL || zappy == NULL || cmd == NULL)
        return ERROR;
    if (cast_action(zappy, client, 7, cmd) == ERROR)
        return ERROR;
    if (check_action(zappy, client) == false)
        return OK;
    if (set_command(zappy, client, cmd) == ERROR)
        return ERROR;
    return OK;
}
