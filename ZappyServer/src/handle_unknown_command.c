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
            dprintf(zappy_server->actual_sockfd,
                "bct %d %d %d %d %d %d %d %d %d\n",
                i, j, zappy_server->map_tile[i][j].food,
                zappy_server->map_tile[i][j].linemate,
                zappy_server->map_tile[i][j].deraumere,
                zappy_server->map_tile[i][j].sibur,
                zappy_server->map_tile[i][j].mendiane,
                zappy_server->map_tile[i][j].phiras,
                zappy_server->map_tile[i][j].thystame);
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

static int ia_client_find_team(zappy_server_t *zappy_server, team_t *tmp_team,
    char *command)
{
    if (strcmp(tmp_team->name, command) == 0) {
        if (tmp_team->nb_matures_eggs == 0) {
            printf("no slot in the team %s left", tmp_team->name);
            return ERROR;
        }
        tmp_team->nb_drones += 1;
        tmp_team->nb_matures_eggs -= 1;
        // mettre bonne valuer ligne en dessous /!/
        dprintf(zappy_server->actual_sockfd, "2 %d %d\n",
            zappy_server->args->width, zappy_server->args->height);
        zappy_server->clients[zappy_server->actual_sockfd].type = IA;
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
