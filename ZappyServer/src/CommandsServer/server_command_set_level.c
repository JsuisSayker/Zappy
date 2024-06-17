/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

void ai_set_level(zappy_server_t *zappy, int id, int level)
{
    for (int i = 3; i < zappy->nb_connected_clients; i += 1) {
        if (zappy->clients[i].client_number == id) {
            zappy->clients[i].level = level;
        }
    }
}

void server_command_set_level(zappy_server_t *zappy, char *command)
{
    int id = 0;
    int level = 1;
    char **command_array = NULL;

    if (command == NULL || command[0] != ' ')
        return;
    command_array = splitter(command, " ");
    if (get_len_char_tab(command_array) != 2)
        return free_array(command_array);
    id = atoi(command_array[0]);
    level = atoi(command_array[1]);
    if (level >= 1 && level <= 8 && id > 0)
        ai_set_level(zappy, id, level);
    free_array(command_array);
}
