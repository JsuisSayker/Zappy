/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** main
*/

#include "first_app.hpp"
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main () {
    lve::FirstApp app{};

    try {
        app.run();
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}