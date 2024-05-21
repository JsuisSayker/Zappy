/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Zappy
*/

#include "Zappy.hpp"

/**
 * @brief Constructs a new instance of the Zappy class.
 */
lve::Zappy::Zappy()
{
}

/**
 * @brief Destructor for the lve::Zappy class.
 * 
 * This destructor is responsible for cleaning up any resources allocated by the lve::Zappy class.
 * 
 * @note This destructor does not perform any specific cleanup actions at the moment.
 */
lve::Zappy::~Zappy()
{
}

void lve::Zappy::run()
{
    while (!lveWindow.shouldClose()) {
        glfwPollEvents();
    }
}
