/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** main
*/

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include "app.hpp"

int main()
{
    tutorial::App app{};

    try {
        app.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}