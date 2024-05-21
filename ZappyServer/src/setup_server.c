/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** setup_server
*/

#include "zappy_server.h"

int setup_server(int port, int max_clients)
{
    int my_socket;
    struct sockaddr_in my_socket_addr;

    my_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (my_socket == KO)
        return KO;
    my_socket_addr.sin_family = AF_INET;
    my_socket_addr.sin_addr.s_addr = INADDR_ANY;
    my_socket_addr.sin_port = htons(port);
    if (bind(my_socket, (const struct sockaddr *)&my_socket_addr,
            sizeof(my_socket_addr)) == KO) {
        return KO;
    }
    if (listen(my_socket, max_clients) == KO)
        return KO;
    return my_socket;
}
