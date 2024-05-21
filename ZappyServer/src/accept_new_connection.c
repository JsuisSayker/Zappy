/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** accept_new_connection
*/

#include <sys/socket.h>
#include <unistd.h>

#include "macro_server.h"

int accept_new_connection(int my_socket)
{
    int client_sockfd = accept(my_socket, NULL, NULL);

    if (client_sockfd == -1) {
        return ERROR;
    }
    return client_sockfd;
}
