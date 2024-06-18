/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Egg
*/

#include "Egg.hpp"

namespace zappy {

Egg::Egg(ZappyGameObject::id_t eggObjectId, int eggNumber, int playerNumber, const glm::vec3 &position)
    : eggObjectId(eggObjectId), eggNumber(eggNumber), playerNumber(playerNumber), position(position)
{
}

} // namespace zappy
