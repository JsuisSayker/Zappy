/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ai_command_right
*/

#include <zappy_server.h>

int take_object(zappy_server_t *zappy, int *tile_object, int *client_object,
    int object_nb)
{
    if ((*tile_object) == 0) {
        dprintf(zappy->actual_sockfd, "ko\n");
        return ERROR;
    }
    (*client_object) += (*tile_object);
    send_pin_command_to_all_gui(zappy, &zappy->clients[zappy->actual_sockfd]);
    send_pgt_command_to_all_gui(zappy, &zappy->clients[zappy->actual_sockfd],
        object_nb);
    (*tile_object) = 0;
    dprintf(zappy->actual_sockfd, "ok\n");
    return OK;
}

static int selector_object_sub(zappy_server_t *zappy, client_t *client,
    char *cmd)
{
    if (strcmp(cmd, "sibur") == OK && take_object(zappy, &zappy->map_tile
    [client->pos.y][client->pos.x].inventory.sibur,
    &client->inventory.sibur, 3) == ERROR)
        return ERROR;
    if (strcmp(cmd, "mendiane") == OK && take_object(zappy, &zappy->map_tile
    [client->pos.y][client->pos.x].inventory.mendiane,
    &client->inventory.mendiane, 4) == ERROR)
        return ERROR;
    if (strcmp(cmd, "phiras") == OK && take_object(zappy, &zappy->map_tile
    [client->pos.y][client->pos.x].inventory.phiras,
    &client->inventory.phiras, 5) == ERROR)
        return ERROR;
    if (strcmp(cmd, "thystame") == OK && take_object(zappy, &zappy->map_tile
    [client->pos.y][client->pos.x].inventory.thystame,
    &client->inventory.thystame, 6) == ERROR)
        return ERROR;
    return OK;
}

static int selector_object(zappy_server_t *zappy, client_t *client, char *cmd)
{
    if (strcmp(cmd, "food") == OK && take_object(zappy, &zappy->map_tile
    [client->pos.y][client->pos.x].inventory.food,
    &client->inventory.food, 0) == ERROR)
        return ERROR;
    if (strcmp(cmd, "linemate") == OK && take_object(zappy, &zappy->map_tile
    [client->pos.y][client->pos.x].inventory.linemate,
    &client->inventory.linemate, 1) == ERROR)
        return ERROR;
    if (strcmp(cmd, "deraumere") == OK && take_object(zappy, &zappy->map_tile
    [client->pos.y][client->pos.x].inventory.deraumere,
    &client->inventory.deraumere, 2) == ERROR)
        return ERROR;
    if (selector_object_sub(zappy, client, cmd) == ERROR)
        return ERROR;
    return OK;
}

void display_inventory(inventory_t *inventory)
{
    printf("Inventory:\n");
    printf("food: %d\n", inventory->food);
    printf("linemate: %d\n", inventory->linemate);
    printf("deraumere: %d\n", inventory->deraumere);
    printf("sibur: %d\n", inventory->sibur);
    printf("mendiane: %d\n", inventory->mendiane);
    printf("phiras: %d\n", inventory->phiras);
    printf("thystame: %d\n", inventory->thystame);
}

int ai_command_take_object(zappy_server_t *zappy, client_t *client, char *cmd)
{
    if (client == NULL || zappy == NULL || cmd == NULL || cmd[4] != ' ')
        return ERROR;
    if (cast_action(zappy, client, 7, cmd) == ERROR)
        return ERROR;
    if (check_action(zappy, client) == false)
        return OK;
    cmd = &cmd[5];
    if (is_valid_resource(cmd) == false)
        return ERROR;
    if (selector_object(zappy, client, cmd) == ERROR)
        return ERROR;
    display_inventory(&client->inventory);
    send_pin_command_to_all_gui(zappy, client);
    return OK;
}
