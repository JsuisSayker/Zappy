/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** int_array_function
*/

#include <zappy_server.h>

int **generate_int_array(int x, int y)
{
    int index = 0;
    int **possible_coordinate = calloc(sizeof(int *), (x * y) + 1);

    for (int i = 0; i < x; i += 1){
        for (int j = 0; j < y; j += 1){
            possible_coordinate[index] = calloc(sizeof(int), 2);
            possible_coordinate[index][0] = i;
            possible_coordinate[index][1] = j;
            index += 1;
        }
    }
    possible_coordinate[index] = NULL;
    return possible_coordinate;
}

void free_int_array(int **possible_coordinate)
{
    for (int i = 0; possible_coordinate[i] != NULL; i += 1) {
        free(possible_coordinate[i]);
    }
    free(possible_coordinate);
}

void shuffle_int_array(int **array, int n)
{
    int *tmp = NULL;
    int index = 0;

    if (n > 1) {
        for (int i = 0; i < n - 1; i++) {
            index = rand() % n;
            tmp = array[index];
            array[index] = array[i];
            array[i] = tmp;
        }
    }
}

void display_int_array(int **array)
{
    for (int i = 0; array[i] != NULL; i += 1) {
        printf("x: %d, y: %d\n", array[i][0], array[i][1]);
    }
}
