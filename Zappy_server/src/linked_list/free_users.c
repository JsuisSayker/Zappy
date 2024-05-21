/*
** EPITECH PROJECT, 2024
** free_list
** File description:
** free the list
*/

#include "zappy_server.h"
#include <stdlib.h>

void free_subscribed(struct subscribedhead *head)
{
    subscribed_t *n1 = NULL;
    subscribed_t *n2 = NULL;

    if (TAILQ_EMPTY(head) == 1) {
        return;
    }
    n1 = TAILQ_FIRST(head);
    while (n1 != NULL) {
        n2 = n1;
        n1 = TAILQ_NEXT(n1, next);
        free(n2);
    }
}

void free_users(struct userhead *head)
{
    user_t *n1 = NULL;
    user_t *n2 = NULL;

    if (TAILQ_EMPTY(head) == 1) {
        return;
    }
    n1 = TAILQ_FIRST(head);
    while (n1 != NULL) {
        n2 = n1;
        n1 = TAILQ_NEXT(n1, next);
        free(n2);
    }
}
