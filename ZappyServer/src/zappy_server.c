/*
** EPITECH PROJECT, 2024
** new_project_folder_file
** File description:
** my_project
*/

#include <zappy_server.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>

static bool loopRunning = true;

void signal_handler(int signal)
{
    if (signal == SIGINT)
        loopRunning = false;
}

static int check_connected_client(zappy_server_t *zappy_server)
{
    if (zappy_server->actual_sockfd == zappy_server->my_socket) {
        return OK;
    } else {
        zappy_server->clients[zappy_server->actual_sockfd].user->nb_clients -=
            1;
        zappy_server->clients[zappy_server->actual_sockfd].user = NULL;
        close(zappy_server->actual_sockfd);
        FD_CLR(zappy_server->actual_sockfd, &zappy_server->fd.save_input);
        printf("client disconnected\n");
    }
    return OK;
}

static int fd_is_set(zappy_server_t *zappy_server)
{
    if (FD_ISSET(zappy_server->actual_sockfd, &zappy_server->fd.input)) {
        if (check_connected_client(zappy_server) == ERROR)
            return ERROR;
        return OK;
    }
    return OK;
}

int send_logout_to_all_clients(zappy_server_t *zappy_server)
{
    for (zappy_server->actual_sockfd = 0;
        zappy_server->actual_sockfd < __FD_SETSIZE;
        zappy_server->actual_sockfd += 1) {
        if (fd_is_set(zappy_server) == ERROR)
            return ERROR;
    }
    return OK;
}

int close_server(zappy_server_t *zappy_server)
{
    send_logout_to_all_clients(zappy_server);
    close(zappy_server->my_socket);
    free_users(&(zappy_server->all_user));
    free_subscribed(&(zappy_server->subscribed_teams_users));
    free_teams(&zappy_server->all_teams);
    free_map_tile(zappy_server->map_tile);
    free_args_config(zappy_server->args);
    free(zappy_server);
    return OK;
}

int zappy_server(args_config_t *args)
{
    zappy_server_t *zappy_server = calloc(sizeof(zappy_server_t), 1);

    signal(SIGINT, signal_handler);
    if (init_server(zappy_server, args) == KO)
        return ERROR;
    while (loopRunning) {
        zappy_server->fd.input = zappy_server->fd.save_input;
        if (select(FD_SETSIZE, &(zappy_server->fd.input),
                &(zappy_server->fd.ouput), NULL, NULL) == KO &&
            loopRunning)
            return ERROR;
        if (loopRunning && scan_fd(zappy_server) == ERROR)
            return ERROR;
    }
    close_server(zappy_server);
    
    return OK;
}
