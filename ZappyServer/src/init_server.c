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
    // FD_SET(STDOUT_FILENO, &fd->save_input);
    FD_ZERO(&fd->ouput);
    FD_SET(my_socket, &fd->ouput);
}

void init_list(zappy_server_t *zappy_server)
{
    TAILQ_INIT(&(zappy_server->all_user));
    zappy_server->all_user.tqh_first = NULL;
    TAILQ_INIT(&(zappy_server->all_teams));
    zappy_server->all_teams.tqh_first = NULL;
    TAILQ_INIT(&(zappy_server->subscribed_teams_users));
    zappy_server->subscribed_teams_users.tqh_first = NULL;
}

int init_server(zappy_server_t *zappy_server, args_config_t *args)
{
    if (zappy_server == NULL) {
        return ERROR;
    }
    zappy_server->my_socket = setup_server(args->port, 42);
    if (zappy_server->my_socket == -1) {
        printf("can't open server port\n");
        free(zappy_server);
        return KO;
    }
    init_fd_struct(&zappy_server->fd, zappy_server->my_socket);
    init_list(zappy_server);
    zappy_server->map_tile = setup_map_tile(args->width, args->height);
    zappy_server->args = args;
    zappy_server->server_running = true;
    return 0;
}
