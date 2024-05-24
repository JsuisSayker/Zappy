/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** handle_unknown_command
*/

#include <zappy_server.h>

int handle_unknown_command(zappy_server_t *zappy_server, char *command)
{
    team_t *tmp_team = NULL;
    if (strcmp("GRAPHIC", command) == 0) {
        zappy_server->clients[zappy_server->actual_sockfd].type = GUI;
        dprintf(zappy_server->actual_sockfd, "msz %d %d\n",
            zappy_server->args->width, zappy_server->args->height);
        for (int i = 0; i < zappy_server->args->height; i++) {
            for (int j = 0; j < zappy_server->args->width; j++) {
                dprintf(zappy_server->actual_sockfd, "bct %d %d %d %d %d %d %d %d %d\n",
                    i, j, zappy_server->map_tile[i][j].food,
                    zappy_server->map_tile[i][j].linemate,
                    zappy_server->map_tile[i][j].deraumere,
                    zappy_server->map_tile[i][j].sibur,
                    zappy_server->map_tile[i][j].mendiane,
                    zappy_server->map_tile[i][j].phiras,
                    zappy_server->map_tile[i][j].thystame);
            }
        }
        return OK;
    } else {
        TAILQ_FOREACH(tmp_team, &zappy_server->all_teams, next) {
            zappy_server->clients[zappy_server->actual_sockfd].type = IA;
            if (strncmp(tmp_team->name, command, strlen(tmp_team->name)) == 0) {
                if (tmp_team->nb_matures_eggs == 0) {
                    printf("no slot in the team %s left", tmp_team->name);
                    return ERROR;
                }
                tmp_team->nb_drones += 1;
                tmp_team->nb_matures_eggs -= 1;
                zappy_server->clients[zappy_server->actual_sockfd].type = IA;
                return OK;
            }
        }
        return ERROR;
    }
    return ERROR;
}
