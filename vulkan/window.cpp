/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** window
*/

#include "window.hpp"

tutorial::Window::Window(int width, int height, std::string name) : width(width), height(height), name(name)
{
    initWindow();
}

tutorial::Window::~Window()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void tutorial::Window::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
}
