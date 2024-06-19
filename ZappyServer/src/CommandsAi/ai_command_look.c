/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ai_command_right
*/

#include "zappy_server.h"

static void object_to_string(char **str, int nb, char *object)
{
    int len = 0;

    for (int i = 0; i < nb; i += 1) {
        len = (strlen((*str)) + strlen(object) + 1);
        (*str) = realloc((*str), sizeof(char) * len);
        (*str) = strcat((*str), object);
        if (i + 1 < nb){
            (*str) = realloc((*str), sizeof(char) * len + 2);
            (*str) = strcat((*str), " ");
        }
    }
}

static void inventory_to_string_sub(inventory_t *inventory, char **str)
{
    object_to_string(str, inventory->mendiane, "mendiane\0");
    if (inventory->mendiane > 0)
        object_to_string(str, 1, " \0");
    object_to_string(str, inventory->phiras, "phiras\0");
    if (inventory->phiras > 0)
        object_to_string(str, 1, " \0");
    object_to_string(str, inventory->thystame, "thystame\0");
    if (inventory->thystame > 0)
        object_to_string(str, 1, " \0");
}

static char *inventory_to_string(inventory_t *inventory)
{
    char *str = calloc(sizeof(char *), 1);

    object_to_string(&str, inventory->food, "food\0");
    if (inventory->food > 0)
        object_to_string(&str, 1, " \0");
    object_to_string(&str, inventory->linemate, "linemate\0");
    if (inventory->linemate > 0)
        object_to_string(&str, 1, " \0");
    object_to_string(&str, inventory->deraumere, "deraumere\0");
    if (inventory->deraumere > 0)
        object_to_string(&str, 1, " \0");
    object_to_string(&str, inventory->sibur, "sibur\0");
    if (inventory->sibur > 0)
        object_to_string(&str, 1, " \0");
    inventory_to_string_sub(inventory, &str);
    return str;
}

static void get_players(zappy_server_t *zappy, look_struct_t *look,
    int x, int y)
{
    for (int i = 3; i < zappy->nb_connected_clients; i ++){
        if (zappy->clients[i].type == IA &&
        (zappy->clients[i].pos.x == x &&
        zappy->clients[i].pos.y == y)
        && look->message[strlen(look->message) - 1] != '[')
            realloc_and_strcat(&look->message, " player ");
        if (zappy->clients[i].type == IA &&
        (zappy->clients[i].pos.x == x &&
        zappy->clients[i].pos.y == y)
        && look->message[strlen(look->message) - 1] == '[')
            realloc_and_strcat(&look->message, "player ");
    }
}

static void look_on_x(zappy_server_t *zappy,
    look_struct_t *look, int lvl)
{
    char *inventory_str;
    int x = 0;
    int y = 0;

    for (int look_x = look->pos.x - lvl; look_x <= look->pos.x + lvl;
        look_x += 1) {
        x = look_x;
        y = look->pos.y;
        if (look->message[strlen(look->message) - 1] != ' '
            && look->message[strlen(look->message) - 1] != '[')
            realloc_and_strcat(&look->message, " \0");
        normalize_coordinate(&x, &y, zappy);
        get_players(zappy, look, x, y);
        inventory_str = inventory_to_string(
            &zappy->map_tile[y][x].inventory);
        realloc_and_strcat(&look->message, inventory_str);
        look->message[strlen(look->message) - 1] = ',';
    }
}

static void look_on_y(zappy_server_t *zappy, look_struct_t *look, int lvl)
{
    char *inventory_str;
    int x = 0;
    int y = 0;

    for (int look_y = look->pos.y - lvl; look_y <= look->pos.y + lvl;
        look_y += 1) {
        x = look->pos.x;
        y = look_y;
        if (look->message[strlen(look->message) - 1] != ' '
            && look->message[strlen(look->message) - 1] != '[')
            realloc_and_strcat(&look->message, " \0");
        normalize_coordinate(&x, &y, zappy);
        get_players(zappy, look, x, y);
        inventory_str = inventory_to_string(
            &zappy->map_tile[y][x].inventory);
        realloc_and_strcat(&look->message, inventory_str);
        look->message[strlen(look->message) - 1] = ',';
    }
}

static void look_direction(zappy_server_t *zappy, look_struct_t *look,
    int lvl)
{
    if (look->pos.direction == NORTH){
        look->pos.y += lvl;
        look_on_x(zappy, look, lvl);
    }
    if (look->pos.direction == EAST){
        look->pos.x += lvl;
        look_on_y(zappy, look, lvl);
    }
    if (look->pos.direction == SOUTH){
        look->pos.y -= lvl;
        look_on_x(zappy, look, lvl);
    }
    if (look->pos.direction == WEST){
        look->pos.x -= lvl;
        look_on_y(zappy, look, lvl);
    }
}

static void send_look_command(zappy_server_t *zappy, client_t *client)
{
    look_struct_t *look = malloc(sizeof(look_struct_t));

    look->message = strdup("[");
    look->pos = client->pos;
    for (int lvl = 0; lvl <= client->level; lvl += 1){
        if (look->message[strlen(look->message) - 1] == ',' &&
            (lvl + 1 < client->level))
            realloc_and_strcat(&look->message, " \0");
        look->pos = client->pos;
        look_direction(zappy, look, lvl);
    }
    if (look->message[strlen(look->message) - 1] == ',')
        look->message[strlen(look->message) - 1] = '\0';
    realloc_and_strcat(&look->message, "]\n");
    dprintf(zappy->actual_sockfd, "%s", look->message);
    free(look->message);
    free(look);
}

int ai_command_look(zappy_server_t *zappy, client_t *client, char *cmd)
{
    printf("    LOOK\n");
    if (client == NULL || zappy == NULL || cmd == NULL)
        return ERROR;
    if (cast_action(zappy, client, 7, cmd) == ERROR)
        return ERROR;
    if (check_action(zappy, client) == false)
        return OK;
    send_look_command(zappy, client);
    printf("    LOOK END\n");
    return OK;
}
