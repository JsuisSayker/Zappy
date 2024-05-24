
#include "zappy_gui.hpp"

// std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main(int ac, char **av) {
  if (ac != 3)
    throw std::runtime_error("Usage: ./zappy_gui <width> <height>");

  zappy::ZappyGui app{std::stoi(av[1]), std::stoi(av[2])};
  try {
    app.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}