/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** GameContent
*/

#pragma once

#include "Client.hpp"
#include "GameObject.hpp"
#include "Map.hpp"
#include "Trantorian.hpp"
#include "ZappyGui.hpp"
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace zappy {
class GameContent : public ZappyGui {
    using FunctionPtr = std::function<void(std::vector<std::string>)>;

  public:
    GameContent(std::shared_ptr<ZappyGui> zappyGui) : zappyGui(zappyGui) {};
    ~GameContent();

    void createTeam(std::shared_ptr<ZappyModel> lveModel,
        const std::string &teamName, const glm::vec3 &position);

    void addTrantorian(std::shared_ptr<ZappyModel> lveModel,
        const std::string &teamName, const glm::vec3 &position);

    // void removeTrantorian(const std::string &teamName,
    // std::shared_ptr<ZappyGameObject> gameObject);

    // void updateNumbers(const std::string &teamName);

    // const std::unordered_map<std::string,
    //     std::vector<Trantorian::trantorian>> &
    // getTeams() const
    // {
    //     return teams_;
    // }

    void setPointerToFunction(
        std::unordered_map<std::string, FunctionPtr> &pointerToFunction);
    std::unordered_map<std::string, FunctionPtr> &getPointerToFunction();

    std::vector<std::vector<std::string>> &getSplitedBuffer();
    void bufferToSplitedBuffer(std::string buffer);

    void setClient(std::shared_ptr<Client> client) { this->client = client; }
    std::shared_ptr<Client> getClient() { return this->client; }

    // Functions to handle commands
    void msz(std::vector<std::string> actualCommand);
    void bct(std::vector<std::string> actualCommand);
    void mct(std::vector<std::string> actualCommand);
    void tna(std::vector<std::string> actualCommand);
    void ppo(std::vector<std::string> actualCommand);
    void plv(std::vector<std::string> actualCommand);
    void pin(std::vector<std::string> actualCommand);
    void sgt(std::vector<std::string> actualCommand);
    void sst(std::vector<std::string> actualCommand);
    void pex(std::vector<std::string> actualCommand);
    void pbc(std::vector<std::string> actualCommand);
    void pic(std::vector<std::string> actualCommand);
    void pie(std::vector<std::string> actualCommand);
    void pfk(std::vector<std::string> actualCommand);
    void pdr(std::vector<std::string> actualCommand);
    void pgt(std::vector<std::string> actualCommand);
    void pdi(std::vector<std::string> actualCommand);
    void enw(std::vector<std::string> actualCommand);
    void eht(std::vector<std::string> actualCommand);
    void ebo(std::vector<std::string> actualCommand);
    void edi(std::vector<std::string> actualCommand);
    void welcome(std::vector<std::string> actualCommand);

  private:
    std::vector<std::vector<std::string>> splitedBuffer_;
    std::unique_ptr<Map> map_;
    std::shared_ptr<Client> client;
    ZappyGameObject::Map gameObjects;
    ZappyGameObject::Map ressources_;
    std::vector<Trantorian> trantorians_;
    std::vector<std::string> splitedBuffer_;
    std::shared_ptr<ZappyGui> zappyGui;
    std::unordered_map<std::string, glm::vec3> teamsColors_;
    std::unordered_map<std::string, FunctionPtr>
        _pointerToFunction; // Map of pointers to functions
};
} // namespace zappy
