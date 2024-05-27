/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** test_zappy_server
*/

#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <zappy_server.h>
#include <signal.h>

Test(zappy_server, test_zappy_server)
{
    // Configuration for the server
    args_config_t *args = calloc(1, sizeof(args_config_t));
    cr_assert_not_null(args);
    args->clientsNb = 2;
    args->port = 4242;
    args->width = 10;
    args->height = 10;
    args->freq = 100;
    TAILQ_INIT(&args->names);
    char_tab_t *names = calloc(1, sizeof(char_tab_t));
    cr_assert_not_null(names);
    names->str = strdup("toto");
    TAILQ_INSERT_TAIL(&args->names, names, next);
    // Forking the process to create a child
    pid_t pid = fork();
    cr_assert(pid >= 0, "Fork failed");
    if (pid == 0) {
        // Child process
        sleep(3);
        kill (getppid(), SIGINT);
        exit(0);
    } else {
        // Parent process
        // Start the server
        int value = zappy_server(args);
        cr_assert_eq(value, 0, "Server did not exit with code 0");
        // Wait for the child process to finish
        int status;
        waitpid(pid, &status, 0);
        cr_assert(WIFEXITED(status) && (WEXITSTATUS(status) == 0),
            "Child process did not exit cleanly");
    }
}
