/*
** EPITECH PROJECT, 2024
** myteams
** File description:
** login_command
*/

#include "zappy_server.h"

int count_str_char(char *str, char c)
{
    int count = 0;

    for (int i = 0; str[i] != '\0'; i += 1) {
        if (str[i] == c) {
            count += 1;
        }
    }
    return count;
}

void generate_new_user(
    zappy_server_t *zappy_server, user_t **new_user, char *username)
{
    *new_user = calloc(sizeof(user_t), 1);
    strcpy((*new_user)->username, username);
    generate_random_uuid((*new_user)->uuid);
    TAILQ_INSERT_TAIL(&zappy_server->all_user, (*new_user), next);
}

void login_command(zappy_server_t *zappy_server, char *command)
{
    user_t *user1 = NULL;
    user_t *user2 = NULL;

    if (strlen(command) == 0 || command[0] != ' ' ||
        count_str_char(command, '\"') != 2) {
        return;
    }
    command = &command[2];
    command[strlen(command) - 1] = '\0';
    for (user1 = zappy_server->all_user.tqh_first; user1 != NULL;
        user1 = user1->next.tqe_next) {
        if (strcmp(user1->username, command) == 0)
            user2 = user1;
    }
    if (user2 == NULL)
        generate_new_user(zappy_server, &user2, command);
    zappy_server->clients[zappy_server->actual_sockfd].user = user2;
    zappy_server->clients[zappy_server->actual_sockfd].user->nb_clients += 1;
    dprintf(zappy_server->actual_sockfd, "200|/login\n%s\n%s\n%s", user2->uuid,
        user2->username, END_STR);
}
