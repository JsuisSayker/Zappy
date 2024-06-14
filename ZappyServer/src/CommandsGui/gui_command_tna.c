/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

void gui_command_tna(zappy_server_t *zappy, char *command)
{
    team_t *tmp_team = NULL;

    if (command[0] != '\0'){
        send_sbp_command_to_all_gui(zappy);
        return;
    }
    TAILQ_FOREACH(tmp_team, &zappy->all_teams, next) {
        dprintf(zappy->actual_sockfd, "tna %s\n", tmp_team->name);
    }
}
