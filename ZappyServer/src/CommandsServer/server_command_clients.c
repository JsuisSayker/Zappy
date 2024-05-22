/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>



void server_command_clients(zappy_server_t *zappy, UNUSED char *command)
{
    printf("================USER================\n");
    for (int i = 3; i < FD_SETSIZE; i += 1){
        if (zappy->clients[i].type != UNKNOWN){
            printf("127.0.0.1:");
            printf("\n");
            printf("Type = ");
            if (zappy->clients[i].type == IA)
                printf("IA\n");
            if (zappy->clients[i].type == GUI)
                printf("GUI\n");
        }
    }
    printf("====================================\n");
}
