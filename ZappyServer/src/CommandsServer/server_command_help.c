/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

void server_command_help(UNUSED zappy_server_t *zappy, UNUSED char *command)
{
    printf("Implemented commands :\n/clients\n"
    "/quit\n/send_ais \"informations\"\n/send_guis \"informations\"\n"
    "/map\n/clear\n/pause\n/start\n/help\n"
    "/setTile ressource quantity x y\n"
    "/tile x y\n/tp id x y\n/kill id\n"
    "/noFood true | false\n/broadcast \"message\" x y\n"
    "/setLevel id level\n/setInventory id ressource quantity\n"
    "/setClientsNb nb\n/setFreq freq\n/noRefill true | false\n"
    "/fork team x y\n/incantate level x y\n");
}
