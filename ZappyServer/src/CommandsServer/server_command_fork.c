/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help
*/

#include <zappy_server.h>

void add_slot_to_team(zappy_server_t *zappy, team_t *team, char **split_cmd)
{
    egg_t *new_egg = calloc(sizeof(egg_t), 1);

    if (new_egg == NULL)
        return;
    new_egg->egg_number = zappy->index_eggs;
    new_egg->client_number = -1;
    new_egg->x = atoi(split_cmd[1]);
    new_egg->y = atoi(split_cmd[2]);
    TAILQ_INSERT_TAIL(&(team->eggs_head), new_egg, next);
    zappy->index_eggs += 1;
    return;
}

void server_command_fork(zappy_server_t *zappy, char *command)
{
    char **split_cmd = NULL;
    team_t *team = NULL;

    if (zappy == NULL || command == NULL)
        return;
    if (command[0] != ' ')
        return;
    command = &command[1];
    split_cmd = splitter(command, " ");
    if (split_cmd == NULL || get_len_char_tab(split_cmd) != 3 ||
        split_cmd[0] == NULL || atoi(split_cmd[1]) < 0 ||
        atoi(split_cmd[1]) > zappy->args->width || atoi(split_cmd[2]) < 0 ||
        atoi(split_cmd[2]) > zappy->args->height)
        return;
    TAILQ_FOREACH(team, &zappy->all_teams, next) {
        if (strcmp(team->name, split_cmd[0]) == 0) {
            add_slot_to_team(zappy, team, split_cmd);
            return;
        }
    }
}
