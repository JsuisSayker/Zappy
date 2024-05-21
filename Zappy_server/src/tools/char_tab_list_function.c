/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** char_tab_list_function
*/

#include "zappy_server.h"

void free_char_tab_list(struct char_tab_head *head)
{
    char_tab_t *n1 = TAILQ_FIRST(head);
    char_tab_t *n2 = NULL;

    while (n1 != NULL) {
        n2 = n1;
        n1 = TAILQ_NEXT(n1, next);
        if (n2->str != NULL) {
            free(n2->str);
            n2->str = NULL;
        }
        free(n2);
    }
}

void display_char_tab_list(struct char_tab_head *head)
{
    char_tab_t *n1 = TAILQ_FIRST(head);

    TAILQ_FOREACH(n1, head, next) { printf("%s\n", n1->str); }
}

int count_char_tab_list(struct char_tab_head *head)
{
    char_tab_t *n1 = TAILQ_FIRST(head);
    int count = 0;

    TAILQ_FOREACH(n1, head, next) { count += 1; }
    return count;
}

void random_char_tab_list(struct char_tab_head *head)
{
    char_tab_t *n1 = TAILQ_FIRST(head);
    char_tab_t *n2 = NULL;
    char_tab_t *n3 = NULL;
    char *tmp = NULL;
    int i = 0;
    int j = 0;

    while (n1 != NULL) {
        n2 = n1;
        n1 = TAILQ_NEXT(n1, next);
        j = rand() % count_char_tab_list(head);
        n3 = TAILQ_FIRST(head);
        for (i = 0; i < j; i++) {
            n3 = TAILQ_NEXT(n3, next);
        }
        tmp = n2->str;
        n2->str = n3->str;
        n3->str = tmp;
    }
}
