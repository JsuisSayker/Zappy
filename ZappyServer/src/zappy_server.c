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

static void signal_handler(UNUSED int signal)
{
}

int get_nb_teams(struct teamhead *head)
{
    int i = 0;
    team_t *tmp;

    TAILQ_FOREACH(tmp, head, next) {
        i += 1;
    }
    return i;
};

static int display_info_server(zappy_server_t *zappy_server)
{
    team_t *tmp;

    if (zappy_server == NULL)
        return ERROR;
    printf("=============Zappy Server=============\n");
    printf("port = %d\n", zappy_server->args->port);
    printf("width = %d\n", zappy_server->args->width);
    printf("height = %d\n", zappy_server->args->height);
    printf("clients_nb = %d\n", zappy_server->args->clientsNb);
    printf("freq = %d\n", zappy_server->args->freq);
    printf("Teams [%d]:\n", get_nb_teams(&zappy_server->all_teams));
    TAILQ_FOREACH(tmp, &zappy_server->all_teams, next){
        printf("name : [%s]\n", tmp->name);
        printf("nb_drones = [%d]\n", tmp->nb_drones);
        printf("nb_matures_eggs = [%d]\n", tmp->nb_matures_eggs);
    }
    printf("=====================================\n");
    return OK;
}

int zappy_server(args_config_t *args)
{
    zappy_server_t *zappy_server = calloc(sizeof(zappy_server_t), 1);

    signal(SIGINT, signal_handler);
    if (init_server(zappy_server, args) == KO)
        return ERROR;
    if (display_info_server(zappy_server) == KO)
        return ERROR;
    while (zappy_server->server_running) {
        zappy_server->fd.input = zappy_server->fd.save_input;
        if (select(FD_SETSIZE, &(zappy_server->fd.input),
            &(zappy_server->fd.ouput), NULL, NULL) == ERROR && errno != EINTR
            && zappy_server->server_running)
            return ERROR;
        if (errno == EINTR)
            zappy_server->server_running = false;
        if (zappy_server->server_running && scan_fd(zappy_server) == ERROR)
            return ERROR;
    }
    close_server(zappy_server);
    return OK;
}
