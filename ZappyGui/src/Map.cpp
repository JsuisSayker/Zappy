/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Map
*/

#include "Map.hpp"

namespace zappy {

/**
 * @brief Constructs a new Map object.
 * 
 * This constructor initializes the Map object with default values for width, height, and map.
 * The width and height are set to 0, and the map is initialized as an empty vector.
 */
Map::Map() {
    this->_width = 0;
    this->_height = 0;
    this->_map = {};
}

/**
 * @brief Destroys the Map object.
 * 
 */
Map::~Map() {}

/**
 * @brief Creates a map with the specified width and height.
 * 
 * @param width The width of the map.
 * @param height The height of the map.
 */
void Map::setWidth(int width) { this->_width = width; }

/**
 * @brief Sets the height of the map.
 * 
 * @param height The height of the map.
 */
void Map::setHeight(int height) { this->_height = height; }

/**
 * @brief Sets the map.
 * 
 * @param map The map to set.
 */
void Map::setMap(std::vector<std::vector<resources>> map) { this->_map = map; }

/**
 * @brief Gets the height of the map.
 * 
 * @return int The height of the map.
 */
int Map::getHeight() const { return this->_height; }

/**
 * @brief Gets the width of the map.
 * 
 * @return int The width of the map.
 */
int Map::getWidth() const { return this->_width; }

/**
 * @brief Gets the map.
 * 
 * @return std::vector<std::vector<resources>> The map.
 */
std::vector<std::vector<resources>> Map::getMap() { return this->_map; }
} // namespace zappy