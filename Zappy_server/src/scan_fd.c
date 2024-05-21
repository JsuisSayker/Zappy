/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** scan_fd
*/

#include "zappy_server.h"

static int check_connection(teams_server_t *teams_server)
{
    int client_fd = 0;

    if (teams_server->actual_sockfd == teams_server->my_socket) {
        client_fd = accept_new_connection(teams_server->my_socket);
        if (client_fd == ERROR) {
            return ERROR;
        }
        dprintf(client_fd, "220 Service ready for new user.\n");
        dprintf(client_fd, END_STR);
        FD_SET(client_fd, &teams_server->fd.save_input);
    } else {
        handle_client(teams_server);
    }
    return OK;
}

static int fd_is_set(teams_server_t *teams_server)
{
    if (FD_ISSET(teams_server->actual_sockfd, &teams_server->fd.input)) {
        if (check_connection(teams_server) == ERROR) {
            return ERROR;
        }
        return OK;
    }
    return OK;
}

int scan_fd(teams_server_t *teams_server)
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
