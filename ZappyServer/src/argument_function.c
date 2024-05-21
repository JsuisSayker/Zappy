/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** argument_function
*/

#include "zappy_server.h"
#include <unistd.h>

args_config_t *init_args_config(void)
{
    args_config_t *args = calloc(sizeof(args_config_t), 1);

    args->port = 0;
    args->width = 0;
    args->height = 0;
    args->clientsNb = 0;
    args->freq = 0.0f;
    TAILQ_INIT(&(args->names));
    return args;
}

// void display_args_config(args_config_t *args)
// {
//     printf("Port: %d\n", args->port);
//     printf("Width: %d\n", args->width);
//     printf("Height: %d\n", args->height);
//     printf("Team Names:\n");
//     display_char_tab_list(&(args->names));
//     printf("Clients Number: %d\n", args->clientsNb);
//     printf("Frequency: %.2f\n", args->freq);
// }

void free_args_config(args_config_t *args)
{
    free_char_tab_list(&(args->names));
    free(args);
}

static int fill_args_conf_three(
    args_config_t *args, int argc, char **argv, int opt)
{
    char_tab_t *tmp = NULL;

    switch (opt) {
    case 'n':
        for (int i = optind - 1; i < argc && *argv[i] != '-'; i++) {
            tmp = calloc(sizeof(char_tab_t), 1);
            tmp->str = strdup(argv[i]);
            TAILQ_INSERT_TAIL(&(args->names), tmp, next);
        }
        break;
    case 'c':
        args->clientsNb = atoi(optarg);
        break;
    case 'f':
        args->freq = atof(optarg);
        break;
    default:
        return KO;
    }
    return OK;
}

static int fill_args_conf_sec(
    args_config_t *args, int argc, char **argv, int opt)
{
    switch (opt) {
    case 'p':
        args->port = atoi(optarg);
        break;
    case 'x':
        args->width = atoi(optarg);
        break;
    case 'y':
        args->height = atoi(optarg);
        break;
    default:
        if (fill_args_conf_three(args, argc, argv, opt) == KO) {
            return KO;
        }
    }
    return OK;
}

static int check_args(args_config_t *args)
{
    if (args->port <= 0 || args->width <= 0 || args->height <= 0 ||
        args->clientsNb == 0 || args->freq == 0.0f) {
        return KO;
    }
    return OK;
}

int fill_args_conf(args_config_t *args, int argc, char **argv)
{
    int opt = getopt(argc, argv, "p:x:y:n:c:f:");

    while (opt != -1) {
        if (fill_args_conf_sec(args, argc, argv, opt) == KO) {
            return KO;
        }
        opt = getopt(argc, argv, "p:x:y:n:c:f:");
    }
    if (check_args(args) == KO) {
        return KO;
    }
    return OK;
}
