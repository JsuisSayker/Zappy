/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Map
*/

#pragma once

#include <vector>

namespace zappy {
    struct resources {
        int food;
        int linemate;
        int deraumere;
        int sibur;
        int mendiane;
        int phiras;
        int thystame;
    };
    class Map {
        public:
            Map();
            ~Map();

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
