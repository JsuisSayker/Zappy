/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Main
*/

#include "Client.hpp"
#include "ZappyGui.hpp"

// std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main(int argc, char **argv)
{
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <path_to_model>" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "------------------Starting ZappyGui------------------"
              << std::endl;
    zappy::ZappyGui app{};
    std::cout << "------------------ZappyGui started------------------"
              << std::endl;
    app.getClient().get()->init(argc, argv);
    app.run();
    return EXIT_SUCCESS;
}
