/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** test_zappy_server
*/

#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <zappy_server.h>

static char *read_client_message(int client_socket)
{
    char buffer[BUFSIZ];
    int n_bytes_read = 0;
    int msg_size = 0;

    n_bytes_read =
        read(client_socket, buffer + msg_size, sizeof(buffer) - msg_size - 1);
    while (n_bytes_read > 0) {
        msg_size += n_bytes_read;
        if (msg_size > BUFSIZ - 1 || buffer[msg_size - 1] == '\n')
            break;
        n_bytes_read = read(
            client_socket, buffer + msg_size, sizeof(buffer) - msg_size - 1);
    }
    if (n_bytes_read == -1)
        return NULL;
    buffer[msg_size] = '\0';
    return strdup(buffer);
}

static void client(char *ip, int port, char *command, int listen_nb)
{
    int server_socket;
    socklen_t server_socket_addr_len;
    struct sockaddr_in server_socket_addr;

    // Create a socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    if (inet_aton(ip, &(server_socket_addr.sin_addr)) <= 0) {
        perror("ERROR invalid address");
        exit(1);
    }

    // Set up the structure~
    server_socket_addr.sin_family = AF_INET;   // Internet address family
    server_socket_addr.sin_port = htons(port); // Any port

    // connect the host address
    server_socket_addr_len = sizeof(server_socket_addr);
    if (connect(server_socket, (const struct sockaddr *)&server_socket_addr,
            server_socket_addr_len) == -1) {
        perror("ERROR on connect");
        exit(1);
    }

    printf("Connected to server\n");
    char *buffer = read_client_message(server_socket);
    printf("server code: %s", buffer);
    free(buffer);
    // Send message to the server
    dprintf(server_socket, "%s\n", command);

    /* Code to deal with incoming connection(s)... */
    for (int i = 0; i < listen_nb; i++) {
        buffer = read_client_message(server_socket);
        printf("server code: |%s|", buffer);
        free(buffer);
    }

    if (close(server_socket) == -1) {
        perror("ERROR on close");
        exit(1);
    }
}

Test(zappy_server, test_zappy_server)
{
    // Configuration for the server
    int sleep_time = 3;
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
        sleep(sleep_time);
        kill(getppid(), SIGINT);
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

Test(zappy_server, test_zappy_server_graphic)
{
    // Configuration for the server
    int sleep_time = 3;
    args_config_t *args = calloc(1, sizeof(args_config_t));
    cr_assert_not_null(args);
    args->clientsNb = 2;
    args->port = 4243;
    args->width = 10;
    args->height = 10;
    args->freq = 100;
    TAILQ_INIT(&args->names);
    char_tab_t *names = calloc(1, sizeof(char_tab_t));
    cr_assert_not_null(names);
    names->str = strdup("toto");
    TAILQ_INSERT_TAIL(&args->names, names, next);
    // Forking the process to create a child
    pid_t pid_1 = fork();
    cr_assert(pid_1 >= 0, "Fork failed");
    if (pid_1 == 0) {
        // Child process
        sleep(sleep_time);
        client("127.0.0.1", args->port, "GRAPHIC", 3);
        exit(0);
    } else {
        pid_t pid_2 = fork();
        cr_assert(pid_2 >= 0, "Fork failed");
        if (pid_2 == 0) {
            // Child process
            sleep(sleep_time * 2);
            kill(getppid(), SIGINT);
            exit(0);
        } else {
            // Parent process
            // Start the server
            int value = zappy_server(args);
            cr_assert_eq(value, 0, "Server did not exit with code 0");
            // Wait for the child process to finish
            int status;
            waitpid(pid_2, &status, 0);
            cr_assert(WIFEXITED(status) && (WEXITSTATUS(status) == 0),
                "Child process did not exit cleanly");
        }
    }
}

Test(zappy_server, test_zappy_server_ai)
{
    // Configuration for the server
    int sleep_time = 3;
    args_config_t *args = calloc(1, sizeof(args_config_t));
    cr_assert_not_null(args);
    args->clientsNb = 2;
    args->port = 4244;
    args->width = 10;
    args->height = 10;
    args->freq = 100;
    TAILQ_INIT(&args->names);
    char_tab_t *names = calloc(1, sizeof(char_tab_t));
    cr_assert_not_null(names);
    names->str = strdup("toto");
    TAILQ_INSERT_TAIL(&args->names, names, next);
    // Forking the process to create a child
    pid_t pid_1 = fork();
    cr_assert(pid_1 >= 0, "Fork failed");
    if (pid_1 == 0) {
        // Child process
        sleep(sleep_time);
        client("127.0.0.1", args->port, "toto", 2);
        exit(0);
    } else {
        pid_t pid_2 = fork();
        cr_assert(pid_2 >= 0, "Fork failed");
        if (pid_2 == 0) {
            // Child process
            sleep(sleep_time * 2);
            kill(getppid(), SIGINT);
            exit(0);
        } else {
            // Parent process
            // Start the server
            int value = zappy_server(args);
            cr_assert_eq(value, 0, "Server did not exit with code 0");
            // Wait for the child process to finish
            int status;
            waitpid(pid_2, &status, 0);
            cr_assert(WIFEXITED(status) && (WEXITSTATUS(status) == 0),
                "Child process did not exit cleanly");
        }
    }
}

Test(zappy_server, test_zappy_server_graphic_ai)
{
    // Configuration for the server
    int sleep_time = 3;
    args_config_t *args = calloc(1, sizeof(args_config_t));
    cr_assert_not_null(args);
    args->clientsNb = 2;
    args->port = 4245;
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
        sleep(sleep_time);
        client("127.0.0.1", args->port, "GRAPHIC", 5);
        exit(0);
    } else {
        pid_t pid = fork();
        cr_assert(pid >= 0, "Fork failed");
        if (pid == 0) {
            // Child process
            sleep(sleep_time * 2);
            client("127.0.0.1", args->port, "toto", 2);
            exit(0);
        } else {
            pid_t pid = fork();
            cr_assert(pid >= 0, "Fork failed");
            if (pid == 0) {
                // Child process
                sleep(sleep_time * 3);
                kill(getppid(), SIGINT);
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
    }
}


// Test(zappy_server, test_zappy_server_graphic_graphic)
// {
//     // Configuration for the server
//     int sleep_time = 3;
//     args_config_t *args = calloc(1, sizeof(args_config_t));
//     cr_assert_not_null(args);
//     args->clientsNb = 2;
//     args->port = 4246;
//     args->width = 10;
//     args->height = 10;
//     args->freq = 100;
//     TAILQ_INIT(&args->names);
//     char_tab_t *names = calloc(1, sizeof(char_tab_t));
//     cr_assert_not_null(names);
//     names->str = strdup("toto");
//     TAILQ_INSERT_TAIL(&args->names, names, next);
//     // Forking the process to create a child
//     pid_t pid = fork();
//     cr_assert(pid >= 0, "Fork failed");
//     if (pid == 0) {
//         // Child process
//         sleep(sleep_time);
//         client("127.0.0.1", args->port, "GRAPHIC", 5);
//         exit(0);
//     } else {
//         pid_t pid = fork();
//         cr_assert(pid >= 0, "Fork failed");
//         if (pid == 0) {
//             // Child process
//             sleep(sleep_time * 2);
//             client("127.0.0.1", args->port, "GRAPHIC", 2);
//             exit(0);
//         } else {
//             pid_t pid = fork();
//             cr_assert(pid >= 0, "Fork failed");
//             if (pid == 0) {
//                 // Child process
//                 sleep(sleep_time * 3);
//                 kill(getppid(), SIGINT);
//                 exit(0);
//             } else {
//                 // Parent process
//                 // Start the server
//                 int value = zappy_server(args);
//                 cr_assert_eq(value, 0, "Server did not exit with code 0");
//                 // Wait for the child process to finish
//                 int status;
//                 waitpid(pid, &status, 0);
//                 cr_assert(WIFEXITED(status) && (WEXITSTATUS(status) == 0),
//                     "Child process did not exit cleanly");
//             }
//         }
//     }
// }