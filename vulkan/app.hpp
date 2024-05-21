/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** app
*/

#pragma once

#include "window.hpp"

namespace tutorial {
    class App {
        public:
            static constexpr int WIDTH = 800;
            static constexpr int HEIGHT = 600;

            void run();
            
        private:
            Window window{WIDTH, HEIGHT, "Vulkan window!"};
    };
}
