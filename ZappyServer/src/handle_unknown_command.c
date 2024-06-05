/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** handle_unknown_command
*/

#include <zappy_server.h>

static void send_egg_info(zappy_server_t *zappy_server)
{
    team_t *tmp_team = NULL;
    egg_t *tmp_egg = NULL;

    TAILQ_FOREACH(tmp_team, &zappy_server->all_teams, next)
    {
        TAILQ_FOREACH(tmp_egg, &tmp_team->eggs_head, next)
        {
            dprintf(zappy_server->actual_sockfd, "enw %d %d %d %d\n",
                tmp_egg->egg_number, tmp_egg->client_number, tmp_egg->x,
                tmp_egg->y);
            dprintf(
                zappy_server->actual_sockfd, "eht %d\n", tmp_egg->egg_number);
        }
    }
}

void send_gui_map_content(map_tile_t **map, int x, int y, int socket)
{
    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++) {
            display_gui_tile(&map[i][j], socket);
        }
    }
}

static void send_map_info(zappy_server_t *zappy_server)
{
    team_t *tmp_team = NULL;

    dprintf(zappy_server->actual_sockfd, "msz %d %d\n",
        zappy_server->args->width, zappy_server->args->height);
    dprintf(zappy_server->actual_sockfd, "sgt %d\n", zappy_server->args->freq);
    send_gui_map_content(zappy_server->map_tile, zappy_server->args->width,
        zappy_server->args->height, zappy_server->actual_sockfd);
    TAILQ_FOREACH(tmp_team, &zappy_server->all_teams, next)
    {
        dprintf(zappy_server->actual_sockfd, "tna %s\n", tmp_team->name);
    }
}

static int graphic_client(zappy_server_t *zappy_server)
{
    zappy_server->clients[zappy_server->actual_sockfd].type = GUI;
    send_map_info(zappy_server);
    send_egg_info(zappy_server);
    return OK;
}

// official GUI don't accept N E W S as direction
// so I put 1 2 3 4 instead
// and command pnw don't have '#'
static void send_info_ai_to_gui(zappy_server_t *zappy_server, client_t *client)
{
    for (int i = 3; i < FD_SETSIZE; i++) {
        if (zappy_server->clients[i].type == GUI) {
            dprintf(i, "pnw %d %d %d %d %d %s\n", client->client_number,
                client->pos.x, client->pos.y, client->pos.direction,
                client->level, client->team_name);
            dprintf(i, "pin %d %d %d %d %d %d %d %d %d %d\n",
                client->client_number, client->pos.x, client->pos.y,
                client->inventory.food, client->inventory.linemate,
                client->inventory.deraumere, client->inventory.sibur,
                client->inventory.mendiane, client->inventory.phiras,
                client->inventory.thystame);
            dprintf(i, "ebo %d\n", 2);
        }
    }
}

static int ai_client_find_team(
    zappy_server_t *zappy_server, team_t *tmp_team, char *command)
{
    client_t *tmp_client = NULL;

    if (zappy_server == NULL || tmp_team == NULL || command == NULL)
        return ERROR;
    if (strncmp(tmp_team->name, command, strlen(tmp_team->name)) == 0) {
        if (tmp_team->nb_matures_eggs == 0) {
            printf("no slot in the team %s left", tmp_team->name);
            return ERROR;
        }
        tmp_client = &zappy_server->clients[zappy_server->actual_sockfd];
        if (ai_initialisation(zappy_server, tmp_client, tmp_team) == ERROR)
            return ERROR;
        dprintf(zappy_server->actual_sockfd, "2\n%d %d\n", tmp_client->pos.x,
            tmp_client->pos.y);
        send_info_ai_to_gui(zappy_server, tmp_client);
        return 1;
    }
    return OK;
}

static int ai_client(zappy_server_t *zappy_server, char *command)
{
    team_t *tmp_team = NULL;
    int team_find = 0;

    TAILQ_FOREACH(tmp_team, &zappy_server->all_teams, next)
    {
        team_find = ai_client_find_team(zappy_server, tmp_team, command);
        if (team_find == 1)
            return OK;
        if (team_find == ERROR)
            return ERROR;
    }
    return ERROR;
}

int handle_unknown_command(zappy_server_t *zappy_server, char *command)
{
    if (strcmp("GRAPHIC", command) == 0)
        return graphic_client(zappy_server);
    else
        return ai_client(zappy_server, command);
    return ERROR;
}
