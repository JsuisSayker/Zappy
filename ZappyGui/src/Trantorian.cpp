/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Trantorian
*/

#include "Trantorian.hpp"

namespace zappy {

/**
 * @brief Constructs a Trantorian object with the specified trantorian object id, point light object id, team, and player number.
 *
 * @param trantObjectId The trantorian object id.
 * @param pointLightObjectId The point light object id.
 * @param team The team.
 * @param playerNumber The player number.
 */
Trantorian::Trantorian(ZappyGameObject::id_t trantObjectId, ZappyGameObject::id_t pointLightObjectId, const std::string &team, int playerNumber)
    : trantorianObject(trantObjectId), pointLightObject(pointLightObjectId), team(team), playerNumber(playerNumber), level(0)
{
    this->inventory = {0, 0, 0, 0, 0, 0, 0};
}

} // namespace zappy
