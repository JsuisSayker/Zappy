/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Window
*/

#include "Window.hpp"
#include "ErrorHandling.hpp"

// std
#include <stdexcept>

namespace zappy {

ZappyWindow::ZappyWindow(int w, int h, std::string name)
    : width{w}, height{h}, windowName{name}
{
    initWindow();
}

ZappyWindow::~ZappyWindow()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void ZappyWindow::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window =
        glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void ZappyWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
  if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
    throw zappy::WindowSurfaceCreationFailedException();
  }
}

void ZappyWindow::framebufferResizeCallback(
    GLFWwindow *window, int width, int height)
{
    auto lveWindow =
        reinterpret_cast<ZappyWindow *>(glfwGetWindowUserPointer(window));
    lveWindow->framebufferResized = true;
    lveWindow->width = width;
    lveWindow->height = height;
}

} // namespace zappy
