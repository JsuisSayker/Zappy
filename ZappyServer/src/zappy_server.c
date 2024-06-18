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

static int display_info_server(zappy_server_t *zappy)
{
    team_t *tmp;

    if (zappy == NULL)
        return ERROR;
    printf("=============Zappy Server=============\n");
    printf("port = %d\n", zappy->args->port);
    printf("width = %d\n", zappy->args->width);
    printf("height = %d\n", zappy->args->height);
    printf("clients_nb = %d\n", zappy->args->clientsNb);
    printf("freq = %d\n", zappy->args->freq);
    printf("Teams [%d]:\n", get_nb_teams(&zappy->all_teams));
    TAILQ_FOREACH(tmp, &zappy->all_teams, next){
        printf("name : [%s]\n", tmp->name);
        printf("nb_drones = [%d]\n", tmp->nb_drones);
        printf("nb_matures_eggs = [%d]\n", tmp->nb_matures_eggs);
    }
    printf("======================================\n");
    return OK;
}

static int init_zappy_server(zappy_server_t *zappy, args_config_t *args)
{
    signal(SIGINT, signal_handler);
    if (init_server(zappy, args) == KO)
        return ERROR;
    if (display_info_server(zappy) == KO)
        return ERROR;
    return OK;
}

static int run_zappy_server(zappy_server_t *zappy)
{
    struct timeval timeout;

    while (zappy->server_running) {
        zappy->fd.input = zappy->fd.save_input;
        timeout.tv_sec = 0;
        timeout.tv_usec = 10000;
        if (select(zappy->nb_connected_clients, &(zappy->fd.input),
            &(zappy->fd.ouput), NULL, &timeout) == ERROR
            && errno != EINTR && zappy->server_running)
            return ERROR;
        if (errno == EINTR)
            zappy->server_running = false;
        if (zappy->server_running && scan_fd(zappy) == ERROR)
            return ERROR;
    }
    close_server(zappy);
    return OK;
}

int zappy_server(args_config_t *args)
{
    zappy_server_t *zappy = calloc(sizeof(zappy_server_t), 1);

    if (init_zappy_server(zappy, args) == ERROR)
        return ERROR;
    return run_zappy_server(zappy);
}
