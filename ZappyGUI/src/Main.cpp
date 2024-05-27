#include <cstdlib>
#include <iostream>
#include <stdexcept>
// #include "ZappyGUI.hpp"

void usage()
{
  std::cerr << "USAGE: ./zappy_gui -p <port> -h <machine>\n  -p\tport number\n  -h\thostname of the server\n";
  std::exit(EXIT_FAILURE);
}

int main(int ac, char **av)
{
  if (std::string(av[1]) == "-help") {
    usage();
    EXIT_SUCCESS;
  }
  if (ac != 3) {
    std::cerr << "Invalid number of arguments\n";
    EXIT_FAILURE;
  }
  //zappy::ZappyGui app{std::stoi(av[1]), std::stoi(av[2])};
  try {
    //app.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}