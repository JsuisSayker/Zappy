/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** test_splitter
*/

#include <criterion/criterion.h>
#include <zappy_server.h>

Test(fill_args_conf, test_fill_args_conf)
{
    args_config_t *args = init_args_config();
    int argc = 15;
    char *argv[16] = {"./zappy_server", "-p", "4242", "-x", "10", "-y", "10", "-n", "toto", "tata", "tutu", "-c", "10", "-f", "10", NULL};
    cr_assert_eq(fill_args_conf(args, argc, argv), OK);
    cr_assert_eq(args->port, 4242);
    cr_assert_eq(args->width, 10);
    cr_assert_eq(args->height, 10);
    cr_assert_eq(args->clientsNb, 10);
    cr_assert_eq(args->freq, 10.0);
    cr_assert_str_eq(args->names.tqh_first->str, "toto");
    cr_assert_str_eq(args->names.tqh_first->next.tqe_next->str, "tata");
    cr_assert_str_eq(args->names.tqh_first->next.tqe_next->next.tqe_next->str, "tutu");
    free_args_config(args);
}