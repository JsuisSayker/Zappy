/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Map
*/

#include "Map.hpp"

namespace zappy {
Map::Map() {}

Map::~Map() {}

void Map::setWidth(int width) { this->_width = width; }

void Map::setHeight(int height) { this->_height = height; }

void Map::setMap(std::vector<std::vector<resources>> map) { this->_map = map; }

int Map::getHeight() const { return this->_height; }

int Map::getWidth() const { return this->_width; }

std::vector<std::vector<resources>> Map::getMap() { return this->_map; }
} // namespace zappy