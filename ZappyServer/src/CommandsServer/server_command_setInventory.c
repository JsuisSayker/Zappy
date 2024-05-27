/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

void server_command_setInventory(zappy_server_t *zappy, char *command)
{
    int id = 0;
    char *resource = NULL;
    int resource_quantity = 0;
    char **command_array = NULL;

    if (command == NULL || command[0] != ' ')
        return;
    command_array = splitter(command, " ");
    if (get_len_char_tab(command_array) != 4)
        return;
    id = atoi(command_array[0]);
    resource = command_array[1];
    resource_quantity = atoi(command_array[2]);
    if (resource_quantity > 0 && id > 0 && is_valid_resource(resource)
        == true) {
        for (int i = 0; i < FD_SETSIZE; i += 1) {
            if (zappy->clients[i].client_number == id) {
                set_inventory_resource_quantite(&zappy->clients[i].inventory,
                resource, resource_quantity);
            }
        }
        printf("setInventory done\n");
    } else {
        printf("setInventory failed\n");
    }
    
}
