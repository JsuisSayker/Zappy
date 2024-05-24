/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** handle_unknown_command
*/

#include <zappy_server.h>


static void send_map_info(zappy_server_t *zappy_server)
{
    team_t *tmp_team = NULL;

    dprintf(zappy_server->actual_sockfd, "msz %d %d\n",
        zappy_server->args->width, zappy_server->args->height);
    for (int i = 0; i < zappy_server->args->height; i++) {
        for (int j = 0; j < zappy_server->args->width; j++) {
            display_gui_tile(zappy_server->map_tile[i][j],
                zappy_server->actual_sockfd);
        }
    }
    TAILQ_FOREACH(tmp_team, &zappy_server->all_teams, next) {
        dprintf(zappy_server->actual_sockfd, "tna %s\n", tmp_team->name);
    }
}

static int graphic_client(zappy_server_t *zappy_server)
{
    zappy_server->clients[zappy_server->actual_sockfd].type = GUI;
    send_map_info(zappy_server);
    return OK;
}

static void send_info_ia_to_gui(zappy_server_t *zappy_server, client_t *client)
{
    for (int i = 3; i < FD_SETSIZE; i++) {
        if (zappy_server->clients[i].type == GUI) {
            dprintf(i, "pnw #%d %d %d %s %d %s\n",
                client->client_number,
                client->pos.x,
                client->pos.y,
                direction_string(client->pos.direction),
                client->level,
                client->team_name);
        }
    }
}

static int ia_value_direction_setter(client_t *ia)
{
    int rdm_orientation = rand() % 3;;

    if (ia == NULL)
        return ERROR;
    if (rdm_orientation == 0)
        ia->pos.direction = NORTH;
    if (rdm_orientation == 1)
        ia->pos.direction = EAST;
    if (rdm_orientation == 2)
        ia->pos.direction = SOUTH;
    if (rdm_orientation == 3)
        ia->pos.direction = WEST;
    return OK;
}

static int ia_value_setter(zappy_server_t *zappy_server, client_t *ia,
    team_t *tmp_team)
{
    int rdm_x = 0;
    int rdm_y = 0;

    if (zappy_server == NULL || ia == NULL || tmp_team == NULL)
        return ERROR;
    rdm_x = rand() % zappy_server->args->width;
    rdm_y = rand() % zappy_server->args->height;
    ia->pos.x = rdm_x;
    ia->pos.y = rdm_y;
    ia->team_name = strdup(tmp_team->name);
    ia->client_number = zappy_server->actual_sockfd;
    ia->type = IA;
    ia->level = 1;
    if (ia_value_direction_setter(ia) == ERROR)
        return ERROR;
    return OK;
}

static int ia_client_find_team(zappy_server_t *zappy_server, team_t *tmp_team,
    char *command)
{
    client_t *tmp_client = NULL;

    if (zappy_server == NULL|| tmp_team == NULL|| command == NULL)
        return ERROR;
    if (strncmp(tmp_team->name, command, strlen(tmp_team->name)) == 0) {
        if (tmp_team->nb_matures_eggs == 0) {
            printf("no slot in the team %s left", tmp_team->name);
            return ERROR;
        }
        tmp_team->nb_drones += 1;
        tmp_team->nb_matures_eggs -= 1;
        tmp_client = &zappy_server->clients[zappy_server->actual_sockfd];
        if (ia_value_setter(zappy_server, tmp_client, tmp_team) == ERROR)
            return ERROR;
        dprintf(zappy_server->actual_sockfd, "2\n%d %d\n",
            tmp_client->pos.x, tmp_client->pos.y);
        send_info_ia_to_gui(zappy_server, tmp_client);
        return 1;
    }
    return OK;
}

static int ia_client(zappy_server_t *zappy_server, char *command)
{
    team_t *tmp_team = NULL;
    int team_find = 0;

    TAILQ_FOREACH(tmp_team, &zappy_server->all_teams, next) {
        team_find = ia_client_find_team(zappy_server, tmp_team, command);
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
        return ia_client(zappy_server, command);
    return ERROR;
}
