/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** window
*/

#pragma once

    #define GLFW_INCLUDE_VULKAN
    #include <string>
    #include <GLFW/glfw3.h>

namespace tutorial {
    class Window {
        public:
            Window(int width, int height, std::string name);
            ~Window();

            Window(const Window &) = delete;
            Window &operator=(const Window &) = delete;

            bool shouldClose() { return glfwWindowShouldClose(window); }

        private:
            void initWindow();

            const int width;
            const int height;
            std::string name;
            GLFWwindow *window;
    };
}