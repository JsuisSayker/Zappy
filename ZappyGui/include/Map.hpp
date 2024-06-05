/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Map
*/

#pragma once

#include <vector>
#include "GameObject.hpp"

namespace zappy {
struct resources {
    std::vector<ZappyGameObject::id_t> food;
    std::vector<ZappyGameObject::id_t> linemate;
    std::vector<ZappyGameObject::id_t> deraumere;
    std::vector<ZappyGameObject::id_t> sibur;
    std::vector<ZappyGameObject::id_t> mendiane;
    std::vector<ZappyGameObject::id_t> phiras;
    std::vector<ZappyGameObject::id_t> thystame;
};
class Map {
  public:
    Map();
    ~Map();

    void createMap(int width, int height);

    void setWidth(int width);
    void setHeight(int height);
    void setMap(std::vector<std::vector<resources>> map);
    int getWidth() const;
    int getHeight() const;
    std::vector<std::vector<resources>> getMap();

  private:
    int _width;
    int _height;
    std::vector<std::vector<resources>> _map; // Modify the name by _tiles
};
} // namespace zappy
