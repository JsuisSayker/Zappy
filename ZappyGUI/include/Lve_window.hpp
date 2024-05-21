/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** lve_window
*/

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace lve
{
    class LveWindow {
        public:
            // constructor and destructor
            LveWindow(int width, int height, std::string windowName);
            ~LveWindow();

            LveWindow(const LveWindow &) = delete;
            LveWindow &operator=(const LveWindow &) = delete;

            // Methods
            void initWindow(int width, int height, std::string windowName);
            bool shouldClose();

        private:
            GLFWwindow* window; // The window object
            int width; // The width of the window
            int height; // The height of the window
            std::string windowName; // The name of the window
    };
}
