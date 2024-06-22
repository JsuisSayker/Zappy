/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ai_function
*/

#include <zappy_server.h>

bool can_i_eat(zappy_server_t *zappy, client_t *client)
{
    struct timeval tv;

    if (zappy == NULL || client == NULL)
        return false;
    if (client->inventory.food == 0)
        return false;
    gettimeofday(&tv, NULL);
    client->health.last_meal = tv.tv_sec + tv.tv_usec / 1000000.0;
    client->inventory.food -= 1;
    send_pin_command_to_all_gui(zappy, client);
    return true;
}

bool is_alive(zappy_server_t *zappy, client_t *client)
{
    struct timeval tv;
    double current_time = 0;

    if (zappy == NULL || client == NULL)
        return false;
    gettimeofday(&tv, NULL);
    current_time = tv.tv_sec + tv.tv_usec / 1000000.0;
    if (current_time - client->health.last_meal > client->health.time_to_eat) {
        if (client->type == AI && can_i_eat(zappy, client) == false) {
            dprintf(zappy->actual_sockfd, "dead\n");
            send_pdi_command_to_all_gui(zappy, client);
            FD_CLR(zappy->actual_sockfd, &zappy->fd.save_input);
            client->type = UNKNOWN;
            return false;
        }
    }
    return true;
}

int ai_function(zappy_server_t *zappy, client_t *client, char *cmd)
{
    printf("    AI FUNCTION\n");
    if (zappy == NULL || client == NULL)
        return ERROR;
    if (is_alive(zappy, client) == false)
        return OK;
    if (handle_ai_command(zappy, client, cmd) == ERROR)
        return ERROR;
    return OK;
}
