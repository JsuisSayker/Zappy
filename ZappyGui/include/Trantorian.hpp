/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Trantorian
*/

#pragma once

#include "Device.hpp"
#include "ErrorHandling.hpp"
#include "GameObject.hpp"
#include "Map.hpp"
#include <algorithm>
#include <glm/vec3.hpp>
#include <memory>
#include <string>
#include <vector>

namespace zappy {
struct trantInventory {
    int food;
    int linemate;
    int deraumere;
    int sibur;
    int mendiane;
    int phiras;
    int thystame;
};
class Trantorian {
  public:
    Trantorian(ZappyGameObject::id_t trantObjectId, ZappyGameObject::id_t pointLightObjectId, const std::string &team, int playerNumber);
    ~Trantorian() = default;

    bool operator==(const Trantorian &trant) const {
        return (this->trantorianObject == trant.trantorianObject);
    }

  public:
    ZappyGameObject::id_t trantorianObject;
    ZappyGameObject::id_t pointLightObject;
    trantInventory inventory;
    std::string team;
    bool incatationInProgess = false;
    int playerNumber;
    int level;
};
} // namespace zappy
