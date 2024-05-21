/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** app
*/

#include "app.hpp"

void tutorial::App::run()
{
    while (!window.shouldClose()) {
        glfwPollEvents();
    }
}
