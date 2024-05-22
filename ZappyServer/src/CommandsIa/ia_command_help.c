/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

int ia_command_help(zappy_server_t *zappy, client_t *client, char *cmd)
{
    (void) cmd;
    if (cast_action(zappy, client, 7) == ERROR)
        return ERROR;
    if (check_action(zappy, client) == false)
        return OK;
    printf("Implemented commands :\n");
    printf("/clients\n");
    printf("/quit\n");
    printf("/send_ais \"informations\"\n");
    printf("/send_guis \"informations\"\n");
    printf("/map\n");
    printf("/clear\n");
    printf("/pause\n");
    printf("/start\n");
    printf("/help\n");
    printf("/setTile ressource quantity x y\n");
    printf("/tile x y\n");
    printf("/tp id x y\n");
    printf("/kill id\n");
    printf("/noFood true | false\n");
    printf("/broadcast \"message\" x y\n");
    printf("/setLevel id level\n");
    printf("/setInventory id ressource quantity\n");
    printf("/setClientsNb nb\n");
    printf("/setFreq freq\n");
    printf("/noRefill true | false\n");
    printf("/fork team x y\n");
    printf("/incantate level x y\n");
    return OK;
}
