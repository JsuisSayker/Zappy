/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

static int do_fork(zappy_server_t *zappy, char **split_cmd)
{
    team_t *team = NULL;
    egg_t *tmp_egg = NULL;

    TAILQ_FOREACH(team, &zappy->all_teams, next) {
        if (strcmp(team->name, split_cmd[0]) == 0) {
            generate_egg_by_team(zappy, team, atoi(split_cmd[1]),
                atoi(split_cmd[2]));
            tmp_egg = TAILQ_LAST(&(team->eggs_head), egghead);
            send_enw_command_to_all_gui(zappy, tmp_egg);
            return OK;
        }
    }
    return KO;
}

void server_command_fork(zappy_server_t *zappy, char *command)
{
    char **split_cmd = NULL;

    if (zappy == NULL || command == NULL || command[0] != ' ')
        return;
    split_cmd = splitter(&command[1], " ");
    if (split_cmd == NULL || get_len_char_tab(split_cmd) != 3 ||
        split_cmd[0] == NULL || atoi(split_cmd[1]) < 0 ||
        atoi(split_cmd[1]) > zappy->args->width || atoi(split_cmd[2]) < 0 ||
        atoi(split_cmd[2]) > zappy->args->height)
        return;
    do_fork(zappy, split_cmd);
}
