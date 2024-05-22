/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** lve_window
*/

#include "Lve_window.hpp"

/**
 * @brief Constructs an LveWindow object with the specified width, height, and
 * window name.
 *
 * @param width The width of the window.
 * @param height The height of the window.
 * @param windowName The name of the window.
 */
lve::LveWindow::LveWindow(int width, int height, std::string windowName)
{
    this->initWindow(width, height, windowName.c_str());
}

/**
 * @brief Destructor for the LveWindow class.
 *
 * This destructor destroys the GLFW window and terminates the GLFW library.
 */
lve::LveWindow::~LveWindow()
{
    glfwDestroyWindow(this->window);
    glfwTerminate();
}

/**
 * @brief Initializes the LveWindow.
 *
 * This function initializes the GLFW library and creates a window with the
 * specified width, height, and window name.
 *
 * @param width The width of the window.
 * @param height The height of the window.
 * @param windowName The name of the window.
 */
void lve::LveWindow::initWindow(int width, int height, std::string windowName)
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    this->window =
        glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
}

/**
 * @brief Checks if the window should close.
 *
 * This function checks if the window should close and returns a boolean value
 * indicating whether the window should close.
 *
 * @return true if the window should close, false otherwise.
 */
bool lve::LveWindow::shouldClose()
{
    return glfwWindowShouldClose(this->window);
}

/**
 * @brief Creates a window surface.
 *
 * This function creates a window surface for the specified Vulkan instance.
 *
 * @param instance The Vulkan instance.
 * @param surface The window surface.
 */
void lve::LveWindow::createWindowSurface(
    VkInstance instance, VkSurfaceKHR *surface)
{
    if (glfwCreateWindowSurface(instance, this->window, nullptr, surface) !=
        VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface!");
    }
}
