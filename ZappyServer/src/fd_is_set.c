/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** fd_is_set
*/

#include <zappy_server.h>

static int check_connection(zappy_server_t *zappy)
{
    int client_fd = 0;

    if (zappy->actual_sockfd == zappy->my_socket) {
        client_fd = accept_new_connection(zappy->my_socket,
            &zappy->clients[zappy->actual_sockfd].other_socket_addr);
        if (client_fd == ERROR)
            return ERROR;
        dprintf(client_fd, "WELCOME\n");
        FD_SET(client_fd, &zappy->fd.save_input);
        zappy->nb_connected_clients += 1;
    } else {
        handle_client(zappy);
    }
    return OK;
}

static void check_win_condition(zappy_server_t *zappy)
{
    team_t *team = NULL;

    TAILQ_FOREACH(team, &zappy->all_teams, next) {
        if (team->nb_clients_lvl_8 >= 6) {
            send_seg_command_to_all_gui(zappy, team->name);
        }
    }
}

int fd_is_set(zappy_server_t *zappy)
{
    client_t *client;

    if (zappy == NULL)
        return ERROR;
    client = &zappy->clients[zappy->actual_sockfd];
    if (FD_ISSET(zappy->actual_sockfd, &zappy->fd.input)) {
        if (check_connection(zappy) == ERROR)
            return ERROR;
        return OK;
    }
    if (client->type == AI) {
        if (ai_function(zappy, client, NULL) == ERROR)
            return ERROR;
    }
    refill_map(zappy);
    check_win_condition(zappy);
    return OK;
}
