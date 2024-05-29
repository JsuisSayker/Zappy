/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** test_splitter
*/

#include <criterion/criterion.h>
#include <zappy_server.h>

Test(get_len_char_tab, test_get_len_char_tab)
{
    char **tab = splitter("hello world", " ");
    cr_assert_eq(get_len_char_tab(tab), 2);
    free_array(tab);
}