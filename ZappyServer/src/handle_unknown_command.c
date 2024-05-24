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

char *direction_string(direction_t orientation)
{
    if (orientation == NORTH)
        return "N";
    if (orientation == EAST)
        return "E";
    if (orientation == SOUTH)
        return "S";
    if (orientation == WEST)
        return "W";
    return "U";
}

static void send_info_ia_to_gui(zappy_server_t *zappy_server, client_t *client)
{
    for (int i = 3; i < FD_SETSIZE; i++) {
        if (zappy_server->clients[i].type == GUI) {
            dprintf(i, "pnw #%d %d %d %s %d %s\n",
                client->client_number,
                client->x,
                client->y,
                direction_string(client->orientation),
                client->level,
                client->team_name);
        }
    }
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
        zappy_server->clients[zappy_server->actual_sockfd].client_number =
            zappy_server->actual_sockfd;
        zappy_server->clients[zappy_server->actual_sockfd].type = IA;
        zappy_server->clients[zappy_server->actual_sockfd].x = rand() %
            zappy_server->args->width;
        zappy_server->clients[zappy_server->actual_sockfd].y = rand() %
            zappy_server->args->height;
        zappy_server->clients[zappy_server->actual_sockfd].orientation = rand()
            % 4;
        zappy_server->clients[zappy_server->actual_sockfd].level = 1;
        zappy_server->clients[zappy_server->actual_sockfd].team_name =
            strdup(tmp_team->name);
        send_info_ia_to_gui(zappy_server, &zappy_server->clients[
            zappy_server->actual_sockfd]);
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
