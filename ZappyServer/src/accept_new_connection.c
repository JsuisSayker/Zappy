/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** accept_new_connection
*/

#include <zappy_server.h>

int accept_new_connection(int my_socket,
    struct sockaddr_in *client_socket_addr)
{
    int client_socket;
    socklen_t client_socket_addr_len;

    client_socket_addr_len = sizeof(*client_socket_addr);
    client_socket = accept(my_socket, (struct sockaddr *)client_socket_addr,
        &client_socket_addr_len);
    if (client_socket == ERROR)
        return ERROR;
    return client_socket;
}
