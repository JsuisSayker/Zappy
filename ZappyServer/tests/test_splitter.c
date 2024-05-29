/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** test_splitter
*/

#include <criterion/criterion.h>
#include <zappy_server.h>

Test(splitter, test_splitter)
{
    char *str = "1 2 3 4";
    char **tab = splitter(str, " ");
    cr_assert_str_eq(tab[0], "1");
    cr_assert_str_eq(tab[1], "2");
    cr_assert_str_eq(tab[2], "3");
    cr_assert_str_eq(tab[3], "4");
    cr_assert_null(tab[4]);
    free_array(tab);
}