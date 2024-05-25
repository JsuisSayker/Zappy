/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** generate_ressourse_list
*/

#include <zappy_server.h>

static bool need_to_generate_more_resource(int i, int x, int y, double density)
{
    return (i < x * y * density / 100 || i < 1);
}

static void add_food_resource(
    struct char_tab_head *resource_list, int x, int y)
{
    char_tab_t *resource = NULL;

    for (int i = 0; need_to_generate_more_resource(i, x, y, FOOD_DENSITY);
        i += 1) {
        resource = calloc(sizeof(char_tab_t), 1);
        resource->str = strdup("food");
        TAILQ_INSERT_TAIL(resource_list, resource, next);
    }
}

static void add_stone_resource_1(
    struct char_tab_head *resource_list, int x, int y)
{
    char_tab_t *resource = NULL;

    for (int i = 0; need_to_generate_more_resource(i, x, y, LINEMATE_DENSITY);
        i += 1) {
        resource = calloc(sizeof(char_tab_t), 1);
        resource->str = strdup("linemate");
        TAILQ_INSERT_TAIL(resource_list, resource, next);
    }
    for (int i = 0; need_to_generate_more_resource(i, x, y, DERAUMERE_DENSITY);
        i += 1) {
        resource = calloc(sizeof(char_tab_t), 1);
        resource->str = strdup("deraumere");
        TAILQ_INSERT_TAIL(resource_list, resource, next);
    }
    for (int i = 0; need_to_generate_more_resource(i, x, y, SIBUR_DENSITY);
        i += 1) {
        resource = calloc(sizeof(char_tab_t), 1);
        resource->str = strdup("sibur");
        TAILQ_INSERT_TAIL(resource_list, resource, next);
    }
}

static void add_stone_resource_2(
    struct char_tab_head *resource_list, int x, int y)
{
    char_tab_t *resource = NULL;

    for (int i = 0; need_to_generate_more_resource(i, x, y, MENDIANE_DENSITY);
        i += 1) {
        resource = calloc(sizeof(char_tab_t), 1);
        resource->str = strdup("mendiane");
        TAILQ_INSERT_TAIL(resource_list, resource, next);
    }
    for (int i = 0; need_to_generate_more_resource(i, x, y, PHIRAS_DENSITY);
        i += 1) {
        resource = calloc(sizeof(char_tab_t), 1);
        resource->str = strdup("phiras");
        TAILQ_INSERT_TAIL(resource_list, resource, next);
    }
    for (int i = 0; need_to_generate_more_resource(i, x, y, THYSTAME_DENSITY);
        i += 1) {
        resource = calloc(sizeof(char_tab_t), 1);
        resource->str = strdup("thystame");
        TAILQ_INSERT_TAIL(resource_list, resource, next);
    }
}

struct char_tab_head *generate_ressourse_list(int x, int y)
{
    struct char_tab_head *resource_list =
        calloc(sizeof(struct char_tab_head), 1);

    TAILQ_INIT(resource_list);
    add_food_resource(resource_list, x, y);
    add_stone_resource_1(resource_list, x, y);
    add_stone_resource_2(resource_list, x, y);
    random_char_tab_list(resource_list);
    return resource_list;
}
