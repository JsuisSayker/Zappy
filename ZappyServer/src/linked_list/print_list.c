/*
** EPITECH PROJECT, 2024
** print_list
** File description:
** print the data of the list
*/

#include "linked_list.h"
#include "my_lib.h"
#include <stdlib.h>

void print_list(linked_list_t *head)
{
    linked_list_t *temp = head;

    for (int i = 0; temp != NULL; temp = temp->next) {
        my_printf("%d\n", temp->data);
    }
}
