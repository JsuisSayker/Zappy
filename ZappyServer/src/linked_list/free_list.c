/*
** EPITECH PROJECT, 2024
** free_list
** File description:
** free the list
*/

#include "zappy_server.h"
#include <stdlib.h>

void free_reply(struct replyhead *head)
{
    reply_t *n1 = TAILQ_FIRST(head);
    reply_t *n2 = NULL;

    while (n1 != NULL) {
        n2 = n1;
        n1 = TAILQ_NEXT(n1, next);
        free(n2);
    }
}

void free_threads(struct threadhead *head)
{
    thread_t *n1 = TAILQ_FIRST(head);
    thread_t *n2 = NULL;

    while (n1 != NULL) {
        n2 = n1;
        if (TAILQ_EMPTY(&n2->replys_head) == 0) {
            free_reply(&n2->replys_head);
        }
        n1 = TAILQ_NEXT(n1, next);
        free(n2);
    }
}

void free_eggs(struct egghead *head)
{
    egg_t *n1 = TAILQ_FIRST(head);
    egg_t *n2 = NULL;

    while (n1 != NULL) {
        n2 = n1;
        n1 = TAILQ_NEXT(n1, next);
        free(n2);
    }
}

void free_teams(struct teamhead *head)
{
    team_t *n1 = TAILQ_FIRST(head);
    team_t *n2 = NULL;

    while (n1 != NULL) {
        n2 = n1;
        n1 = TAILQ_NEXT(n1, next);
        free(n2->name);
        free_eggs(&n2->eggs_head);
        free(n2);
    }
}
