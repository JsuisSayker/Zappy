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

/**
 * @brief Constructs a ZappyWindow object with the specified width, height, and name.
 *
 * @param w The width of the window.
 * @param h The height of the window.
 * @param name The name of the window.
 */
ZappyWindow::ZappyWindow(int w, int h, std::string name)
    : width{w}, height{h}, windowName{name}
{
    initWindow();
}

/**
 * @brief Destroys the ZappyWindow object.
 *
 */
ZappyWindow::~ZappyWindow()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

/**
 * @brief Initializes the window.
 *
 */
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


/**
 * Creates a Vulkan surface for the window.
 *
 * @param instance The Vulkan instance.
 * @param surface Pointer to the Vulkan surface to be created.
 * @throws zappy::WindowSurfaceCreationFailedException if the surface creation fails.
 */
void ZappyWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
  if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
    throw zappy::WindowSurfaceCreationFailedException();
  }
}

/**
 * @brief Callback function for when the framebuffer is resized.
 *
 * @param window The window.
 * @param width The new width.
 * @param height The new height.
 */
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
