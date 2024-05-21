/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Zappy
*/

#pragma once

#include "Lve_window.hpp"

namespace lve
{
    class Zappy {
        public:
            Zappy();
            ~Zappy();

        const int height = 800; // The height of the window
        const int width = 600; // The width of the window

        void run();

        private:
            LveWindow lveWindow{width, width, "Zappy"}; // The window object
    };
}
