/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Trantorian
*/

#include "Trantorian.hpp"

namespace zappy {

Trantorian::Trantorian(ZappyGameObject::id_t trantObjectId, ZappyGameObject::id_t pointLightObjectId, const std::string &team, int playerNumber)
    : trantorianObject(trantObjectId), pointLightObject(pointLightObjectId), team(team), playerNumber(playerNumber), level(0)
{
    this->inventory = {0, 0, 0, 0, 0, 0, 0};
}

} // namespace zappy
