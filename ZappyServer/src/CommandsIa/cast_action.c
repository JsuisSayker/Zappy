/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** cast_action
*/

#include <zappy_server.h>
#include <sys/time.h>

int cast_action(zappy_server_t *zappy, client_t *client, int freq)
{
    if (client == NULL || zappy == NULL)
        return ERROR;
    if (client->is_contracted == false) {
        client->is_contracted = true;
        client->freq = freq / zappy->args->freq;
        gettimeofday(&client->start, NULL);
    }
    return OK;
}

bool check_action(zappy_server_t *zappy, client_t *client)
{
    struct timeval end;
    double elapsed;

    if (client == NULL || zappy == NULL)
        return false;
    gettimeofday(&end, NULL);
    elapsed = end.tv_sec - client->start.tv_sec;
    if (client->is_contracted == true && client->freq < elapsed) {
        client->is_contracted = false;
        client->freq = 0;
        client->start.tv_sec = 0;
        return true;
    }
    return false;
}
