/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** cast_action
*/

#include <zappy_server.h>

int cast_action(zappy_server_t *zappy, client_t *client, int freq)
{
    if (client == NULL || zappy == NULL)
        return ERROR;
    if (client->is_contracted == false) {
        client->is_contracted = true;
        client->freq = freq;
        client->start_timer = clock();
    }
    return OK;
}

bool check_action(zappy_server_t *zappy, client_t *client)
{
    int frequence = 0;

    if (client == NULL || zappy == NULL)
        return false;
    frequence = client->freq / zappy->args->freq;
    if (client->is_contracted == true &&  frequence >
    (clock() - client->start_timer) / CLOCKS_PER_SEC) {
        client->is_contracted = false;
        client->start_timer = 0;
        client->freq = 0;
        return true;
    }
    return false;
}