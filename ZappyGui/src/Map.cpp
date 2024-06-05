/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Map
*/

#include "Map.hpp"

namespace zappy {
Map::Map()
{
}

Map::~Map()
{
}

void Map::createMap(int width, int height)
{
    for (float i = 0.f; i < width; i++) {
        for (float j = 0.f; j < height; j++) {
            createGameObject(executablePath + "/ZappyGui/models/Grass_Block.obj",
        executablePath + "/ZappyGui/textures/Grass_Block.png", {i, j, 0.f},
        {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f});
        }
    }
}
} // namespace zappy
