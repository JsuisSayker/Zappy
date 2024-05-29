/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>
#include <stdio.h>

int ai_command_help(zappy_server_t *zappy, client_t *client, char *cmd)
{
    (void)zappy;
    (void)cmd;
    if (client->command.execusion != NULL){
        free(client->command.execusion);
        client->command.execusion = NULL;
    }
    dprintf(client->client_number, "Implemented commands :\n/clients\n"
    "/quit\n/send_ai_s \"informations\"\n/send_guis \"informations\"\n"
    "/map\n/clear\n/pause\n/start\n/help\n"
    "/setTile ressource quantity x y\n"
    "/tile x y\n/tp id x y\n/kill id\n"
    "/noFood true | false\n/broadcast \"message\" x y\n"
    "/setLevel id level\n/setInventory id ressource quantity\n"
    "/setClientsNb nb\n/setFreq freq\n/noRefill true | false\n"
    "/fork team x y\n/incantate level x y\n");
    return OK;
}
