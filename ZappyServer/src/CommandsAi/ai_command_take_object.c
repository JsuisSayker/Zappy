/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ai_command_right
*/

#include <zappy_server.h>

int take_object(int *tile_object, int *client_object, int socket)
{
        if ((*tile_object) == 0) {
            dprintf(socket, "ko\n");
            return ERROR;
        }
        (*tile_object) -= 1;
        (*client_object) += 1;
        dprintf(socket, "ok\n");
        return OK;
}

static int selector_object_sub(zappy_server_t *zappy, client_t *client,
    char *cmd)
{
    if (strcmp(cmd, "sibur") == OK && take_object(&zappy->map_tile
    [client->pos.y][client->pos.x].inventory.sibur,
    &client->inventory.sibur, zappy->actual_sockfd) == ERROR)
        return ERROR;
    if (strcmp(cmd, "mendiane") == OK && take_object(&zappy->map_tile
    [client->pos.y][client->pos.x].inventory.mendiane,
    &client->inventory.mendiane, zappy->actual_sockfd) == ERROR)
        return ERROR;
    if (strcmp(cmd, "phiras") == OK && take_object(&zappy->map_tile
    [client->pos.y][client->pos.x].inventory.phiras,
    &client->inventory.phiras, zappy->actual_sockfd) == ERROR)
        return ERROR;
    if (strcmp(cmd, "thystame") == OK && take_object(&zappy->map_tile
    [client->pos.y][client->pos.x].inventory.thystame,
    &client->inventory.thystame, zappy->actual_sockfd) == ERROR)
        return ERROR;
    return OK;
}

static int selector_object(zappy_server_t *zappy, client_t *client, char *cmd)
{
    if (strcmp(cmd, "food") == OK && take_object(&zappy->map_tile
    [client->pos.y][client->pos.x].inventory.food,
    &client->inventory.food, zappy->actual_sockfd) == ERROR)
        return ERROR;
    if (strcmp(cmd, "linemate") == OK && take_object(&zappy->map_tile
    [client->pos.y][client->pos.x].inventory.linemate,
    &client->inventory.linemate, zappy->actual_sockfd)
        == ERROR)
        return ERROR;
    if (strcmp(cmd, "deraumere") == OK && take_object(&zappy->map_tile
    [client->pos.y][client->pos.x].inventory.deraumere,
    &client->inventory.deraumere, zappy->actual_sockfd) == ERROR)
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
    if (client == NULL || zappy == NULL || cmd == NULL || cmd[0] != ' ')
        return ERROR;
    cmd = &cmd[1];
    if (cast_action(zappy, client, 7, cmd) == ERROR)
        return ERROR;
    printf("Left\n");
    if (check_action(zappy, client) == false)
        return OK;
    if (is_valid_resource(cmd) == false)
        return ERROR;
    if (selector_object(zappy, client, cmd) == ERROR)
        return ERROR;
    display_inventory(&client->inventory);
    send_pin_command_to_all_gui(zappy, client);
    return OK;
}
