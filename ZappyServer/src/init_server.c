/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** init_fd_struct
*/

#include <zappy_server.h>

static void init_fd_struct(fd_t *fd, int my_socket)
{
    FD_ZERO(&fd->save_input);
    FD_SET(my_socket, &fd->save_input);
    FD_SET(STDIN_FILENO, &fd->save_input);
    FD_ZERO(&fd->ouput);
    FD_SET(my_socket, &fd->ouput);
}

static void init_list(zappy_server_t *zappy)
{
    TAILQ_INIT(&(zappy->all_teams));
    zappy->all_teams.tqh_first = NULL;
}

static void generate_egg_by_team(zappy_server_t *zappy,
    team_t *new_team)
{
    egg_t *new_egg = NULL;

    for (int i = 0; i < zappy->args->clientsNb; i += 1) {
        new_egg = calloc(sizeof(egg_t), 1);
        if (new_egg == NULL)
            return;
        new_egg->egg_number = zappy->index_eggs;
        new_egg->client_number = -1;
        new_egg->x = rand() % zappy->args->width;
        new_egg->y = rand() % zappy->args->height;
        TAILQ_INSERT_TAIL(&(new_team->eggs_head), new_egg, next);
        zappy->index_eggs += 1;
    }
}

static void create_teams(zappy_server_t *zappy)
{
    team_t *new_team = NULL;
    char_tab_t *team_name = NULL;

    team_name = TAILQ_FIRST(&(zappy->args->names));
    while (team_name != NULL){
        new_team = calloc(sizeof(team_t), 1);
        generate_random_uuid(new_team->team_uuid);
        new_team->name = calloc(sizeof(char), strlen(team_name->str) + 1);
        if (new_team->name == NULL)
            return;
        strcpy(new_team->name, team_name->str);
        new_team->nb_drones = 0;
        new_team->nb_matures_eggs = zappy->args->clientsNb;
        TAILQ_INIT(&(new_team->eggs_head));
        generate_egg_by_team(zappy, new_team);
        TAILQ_INSERT_TAIL(&(zappy->all_teams), new_team, next);
        team_name = TAILQ_NEXT(team_name, next);
    }
}

static void init_value(zappy_server_t *zappy)
{
    zappy->index_eggs = 0;
    zappy->index_clients = 0;
    zappy->server_running = true;
    zappy->time_refill_map = time(NULL);
    init_list(zappy);
    create_teams(zappy);
    zappy->map_tile = setup_map_tile(zappy->args->width,
        zappy->args->height);
    zappy->map_tile_save = copy_map_tile(zappy->map_tile);
    for (int i = 0; i < FD_SETSIZE; i += 1) {
        zappy->clients[i].type = UNKNOWN;
        zappy->clients[i].client_number = -1;
        zappy->clients[i].team_name = NULL;
    }
}

int init_server(zappy_server_t *zappy, args_config_t *args)
{
    if (zappy == NULL)
        return ERROR;
    zappy->my_socket = setup_server(args->port, 42);
    if (zappy->my_socket == -1) {
        printf("can't open server port\n");
        free(zappy);
        return KO;
    }
    init_fd_struct(&zappy->fd, zappy->my_socket);
    zappy->args = args;
    init_value(zappy);
    return OK;
}
