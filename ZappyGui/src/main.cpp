
#include "zappy_gui.hpp"

// std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {
  zappy::FirstApp app{};

  try {
    app.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}