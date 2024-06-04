/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** GameContent
*/

#pragma once

#include "GameObject.hpp"
#include "Map.hpp"
#include "Trantorian.hpp"
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

namespace zappy {
class GameContent {
    using FunctionPtr = std::function<void(void)>;

  public:
    GameContent();
    ~GameContent();
    void addTeam(const std::string &teamName);

    void addTrantorian(const std::string &teamName,
        std::shared_ptr<ZappyGameObject> gameObject, int number);

    void removeTrantorian(const std::string &teamName,
        std::shared_ptr<ZappyGameObject> gameObject);

    void updateNumbers(const std::string &teamName);

    // const std::unordered_map<std::string,
    //     std::vector<Trantorian::trantorian>> &
    // getTeams() const
    // {
    //     return teams_;
    // }

    void setPointerToFunction(
        std::unordered_map<std::string, FunctionPtr> &pointerToFunction);
    std::unordered_map<std::string, FunctionPtr> &getPointerToFunction();

    // Functions to handle commands
    void msz();

  private:
    std::unique_ptr<Map> map_;
    ZappyGameObject::Map ressources_;
    std::vector<Trantorian> trantorians_;
    std::unordered_map<std::string, glm::vec3> teamsColors_;
    std::unordered_map<std::string, FunctionPtr>
        _pointerToFunction; // Map of pointers to functions
};
} // namespace zappy
