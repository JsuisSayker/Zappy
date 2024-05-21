/*
** EPITECH PROJECT, 2024
** new_project_folder_file
** File description:
** my_project
*/

#include "zappy_server.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>

static bool loopRunning = true;

void signal_handler(int signal)
{
    if (signal == SIGINT) {
        loopRunning = false;
    }
}

static int check_connected_client(teams_server_t *teams_server)
{
    if (teams_server->actual_sockfd == teams_server->my_socket) {
        return OK;
    } else {
        logout_command(teams_server, "");
        printf("client disconnected\n");
    }
    return OK;
}

static int fd_is_set(teams_server_t *teams_server)
{
    if (FD_ISSET(teams_server->actual_sockfd, &teams_server->fd.input)) {
        if (check_connected_client(teams_server) == ERROR) {
            return ERROR;
        }
        return OK;
    }
    return OK;
}

int send_logout_to_all_clients(teams_server_t *teams_server)
{
    for (teams_server->actual_sockfd = 0;
        teams_server->actual_sockfd < __FD_SETSIZE;
        teams_server->actual_sockfd += 1) {
        if (fd_is_set(teams_server) == ERROR) {
            return ERROR;
        }
    }
    return OK;
}

int close_server(teams_server_t *teams_server)
{
    save_info_to_file(teams_server);
    send_logout_to_all_clients(teams_server);
    close(teams_server->my_socket);
    free_users(&(teams_server->all_user));
    free_messages(&(teams_server->private_messages));
    free_subscribed(&(teams_server->subscribed_teams_users));
    free_teams(&teams_server->all_teams);
    free(teams_server);
    return OK;
}

int zappy_server(args_config_t *args)
{
    teams_server_t *teams_server = calloc(sizeof(teams_server_t), 1);

    signal(SIGINT, signal_handler);
    if (init_server(teams_server, args->port) == KO) {
        return ERROR;
    }
    read_info_from_save_file(teams_server);
    while (loopRunning) {
        teams_server->fd.input = teams_server->fd.save_input;
        if (select(FD_SETSIZE, &(teams_server->fd.input),
                &(teams_server->fd.ouput), NULL, NULL) == KO &&
            loopRunning) {
            return ERROR;
        }
        if (loopRunning && scan_fd(teams_server) == ERROR) {
            return ERROR;
        }
    }
    close_server(teams_server);
    return OK;
}
