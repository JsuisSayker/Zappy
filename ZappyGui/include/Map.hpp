/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Map
*/

#pragma once

#include <vector>

namespace zappy {
    struct ressources {
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

        private:
            int _width;
            int _height;
            std::vector<std::vector<ressources>> _map;
    };
}
