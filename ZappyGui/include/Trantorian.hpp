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

    // void updateTrantorian(const Trantorian &trant);

    // void updateTrantorianTranslation(const Trantorian &trant, glm::vec3
    // translation);

    // void updateTrantorianRotation(const Trantorian &trant, glm::vec3
    // rotation);

    // void updateTrantorianLevel(const std::string &team, ZappyGameObject
    // gameObject);

    // void updateInventory(const std::string &team, ZappyGameObject
    // gameObject, const std::string &item, bool operator_);

    // std::unordered_map<std::string, int> getInventory(const std::string
    // &team, ZappyGameObject gameObject) const;

    // ZappyGameObject::id_t getTrantorianGameObjectId(const std::string &team,
    // ZappyGameObject gameObject) const;

    // const std::vector<Trantorian> &getTrantorians() const { return
    // Trantorians_; }

  private:
    ZappyGameObject::id_t trantorianObject;
    ZappyGameObject::id_t pointLightObject;
    trantInventory inventory;
    std::string team;
    int playerNumber;
    int level;
};
} // namespace zappy
