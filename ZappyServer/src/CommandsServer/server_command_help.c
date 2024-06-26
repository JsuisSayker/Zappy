/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

void server_command_help(zappy_server_t *zappy, char *command)
{
    if (zappy == NULL || command == NULL || command[0] != '\0')
        return;
    printf("Implemented commands :\n/clients\n"
    "/quit\n/send_ai_s \"informations\"\n/send_guis \"informations\"\n"
    "/map\n/clear\n/pause\n/start\n/help\n"
    "/setTile resource quantity x y\n"
    "/tile x y\n/tp id x y\n/kill id\n"
    "/noFood true | false\n/broadcast \"message\" x y\n"
    "/setLevel id level\n/setInventory id resource quantity\n"
    "/setClientsNb nb\n/setFreq freq\n/noRefill true | false\n"
    "/fork team x y\n/incantate level x y\n");
}
