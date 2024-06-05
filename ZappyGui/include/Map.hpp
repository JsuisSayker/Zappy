/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Map
*/

#pragma once

#include <vector>
#include <vector>
#include "GameContent.hpp"

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
    class Map: public GameContent {
        public:
            Map();
            ~Map();

            void createMap(int width, int height);

            // void setWidth(int width) { _width = width; }
            // void setHeight(int height) { _height = height; }
            // void setMap(std::vector<std::vector<resources>> map) { _map = map; }
            // int getWidth() const { return _width; }
            // int getHeight() const { return _height; }
            // std::vector<std::vector<resources>> getMap() const { return _map; }

        private:
            int _width;
            int _height;
            std::vector<std::vector<resources>> _map;
    };
}
