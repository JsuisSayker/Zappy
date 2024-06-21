/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Egg
*/

#include "Egg.hpp"

namespace zappy {

/**
 * @brief Constructs a new Egg object.
 *
 * @param eggObjectId The ID of the egg object.
 * @param eggNumber The number of the egg.
 * @param playerNumber The number of the player who laid the egg.
 * @param position The position of the egg in 3D space.
 */
Egg::Egg(ZappyGameObject::id_t eggObjectId, int eggNumber, int playerNumber, const glm::vec3 &position)
    : eggObjectId(eggObjectId), eggNumber(eggNumber), playerNumber(playerNumber), position(position)
{
}

} // namespace zappy
