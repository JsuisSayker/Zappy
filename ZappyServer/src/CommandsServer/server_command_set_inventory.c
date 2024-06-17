/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

static void set_inventory(zappy_server_t *zappy, int id, char *resource,
    int resource_quantity)
{
    for (int i = 3; i < zappy->nb_connected_clients; i += 1) {
        if (zappy->clients[i].client_number == id) {
            set_inventory_resource_quantite(&zappy->clients[i].inventory,
            resource, resource_quantity);
        }
    }
}

void server_command_set_inventory(zappy_server_t *zappy, char *command)
{
    int id = 0;
    char *resource = NULL;
    int resource_quantity = 0;
    char **command_array = NULL;

    if (command == NULL || command[0] != ' ')
        return;
    command_array = splitter(command, " ");
    if (get_len_char_tab(command_array) != 3)
        return free_array(command_array);
    id = atoi(command_array[0]);
    resource = command_array[1];
    resource_quantity = atoi(command_array[2]);
    if (resource_quantity > 0 && id > 0 && is_valid_resource(resource)
        == true)
        set_inventory(zappy, id, resource, resource_quantity);
    free_array(command_array);
}
