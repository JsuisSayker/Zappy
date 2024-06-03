/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ai_command_right
*/

#include <zappy_server.h>

static int **get_coordinate_ai(zappy_server_t *zappy)
{
    int nb_ai = 0;
    for (int i = 0; i < FD_SETSIZE; i++) {
        if (zappy->clients[i].type == IA)
            nb_ai++;
    }
    int **coordinate_ai = generate_int_array(nb_ai, 1);
    int index = 0;
    for (int i = 0; i < FD_SETSIZE; i += 1) {
        if (zappy->clients[i].type == IA) {
            coordinate_ai[index][0] = zappy->clients[i].pos.x;
            coordinate_ai[index][1] = zappy->clients[i].pos.y;
            index++;
        }
    }
    return coordinate_ai;
}

static void normalize_coordinate(int *x, int *y, zappy_server_t *zappy)
{
    if (*x < 0)
        *x = zappy->args->width - 1;
    if (*y < 0)
        *y = zappy->args->height - 1;
    if (*x >= zappy->args->width)
        *x -= zappy->args->width;
    if (*y >= zappy->args->height)
        *y -= zappy->args->height;
}

static void object_to_string(char **str, int nb, char *object)
{
    for (int i = 0; i < nb; i += 1) {
        (*str) = realloc((*str), sizeof(char) * (strlen((*str)) + strlen(object)));
        (*str) = strcat((*str), object);
    }
}

static char *inventory_to_string(inventory_t *inventory)
{
    char *str = calloc(sizeof(char *), 1);

    object_to_string(&str, inventory->food, " food");
    object_to_string(&str, inventory->linemate, " linemate");
    object_to_string(&str, inventory->deraumere, " deraumere");
    object_to_string(&str, inventory->sibur, " sibur");
    object_to_string(&str, inventory->mendiane, " mendiane");
    object_to_string(&str, inventory->phiras, " phiras");
    object_to_string(&str, inventory->thystame, " thystame");
    return str;
}

static void realloc_and_strcat(char **message, char *str)
{
    *message = realloc(*message, sizeof(char) * (strlen(*message) + strlen(str)));
    *message = strcat(*message, str);
}

static void send_look_command_to_client(zappy_server_t *zappy,
    client_t *client)
{
    int x = client->pos.x;
    int y = client->pos.y;
    int **coordinate_ai = get_coordinate_ai(zappy);

    char *message = strdup("[");
    for (int k = 0; k < client->level + 1; k += 1) {
        for (int i = x - k; i < x + k; i += 1) {
            for (int j = y - k; j < y + k; j += 1) {
                normalize_coordinate(&i, &j, zappy);
                for (int l = 0; coordinate_ai[l] != NULL; l += 1){
                    if (coordinate_ai[l][0] == i && coordinate_ai[l][1] == j)
                        realloc_and_strcat(&message, " player");
                }
                char *inventory_str = inventory_to_string(
                    &zappy->map_tile[j][i].inventory);
                realloc_and_strcat(&message, inventory_str);
                free(inventory_str);
                if (i != x + k - 1 || j != y + k - 1)
                    realloc_and_strcat(&message, ",");
            }
        }
    }
    realloc_and_strcat(&message, "]\n");
    dprintf(zappy->actual_sockfd, message);
    free_int_array(coordinate_ai);
    free(message);
}

int ai_command_look(zappy_server_t *zappy, client_t *client, char *cmd)
{
    if (client == NULL || zappy == NULL || cmd == NULL)
        return ERROR;
    if (cast_action(zappy, client, 7, cmd) == ERROR)
        return ERROR;
    if (check_action(zappy, client) == false)
        return OK;
    send_look_command_to_client(zappy, client);
    return OK;
}
