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

static int display_info_server(zappy_server_t *zappy_server)
{
    if (zappy_server == NULL)
        return ERROR;
    printf("=============Zappy Server=============\n");
    printf("port = %d\n", zappy_server->args->port);
    printf("width = %d\n", zappy_server->args->width);
    printf("height = %d\n", zappy_server->args->height);
    printf("clients_nb = %d\n", zappy_server->args->clientsNb);
    printf("freq = %f\n", zappy_server->args->freq);
    printf("Teams [%s]:\n", "Not setup yet");
    printf("display eggs = [%s]\n", "Not setup yet");
    printf("name : [%s]\n", "Not setup yet");
    printf("nb_drones = [%s]\n", "Not setup yet");
    printf("nb_eggs = [%s]\n", "Not setup yet");
    printf("verbose = [%s]\n", "Not setup yet");
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
    while (loopRunning && zappy_server->server_running) {
        zappy_server->fd.input = zappy_server->fd.save_input;
        if (select(FD_SETSIZE, &(zappy_server->fd.input),
            &(zappy_server->fd.ouput), NULL, NULL) == KO && loopRunning)
            return ERROR;
        if (loopRunning && scan_fd(zappy_server) == ERROR)
            return ERROR;
    }
    close_server(zappy_server);
    return OK;
}
