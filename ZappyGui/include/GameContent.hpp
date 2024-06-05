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
#include "ZappyGui.hpp"
#include "Client.hpp"
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace zappy {
class GameContent: public ZappyGui {
    using FunctionPtr = std::function<void(void)>;

  public:
    GameContent(std::shared_ptr<ZappyGui> zappyGui) : zappyGui(zappyGui) {};
    ~GameContent();
    
    void createTeam(std::shared_ptr<ZappyModel> lveModel, const std::string &teamName, const glm::vec3 &position);

    void addTrantorian(std::shared_ptr<ZappyModel> lveModel, const std::string &teamName, const glm::vec3 &position);

    // void removeTrantorian(const std::string &teamName, std::shared_ptr<ZappyGameObject> gameObject);

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

    std::vector<std::string> &getSplitedBuffer();
    void setSplitedBuffer(std::string splitedBuffer);

    void setClient(std::shared_ptr<Client> client) { this->client = client; }
    std::shared_ptr<Client> getClient() { return this->client; }

    // Functions to handle commands
    void msz();
    void bct();
    void mct();
    void tna();
    void ppo();
    void plv();
    void pin();
    void sgt();
    void sst();
    void pex();
    void pbc();
    void pic();
    void pie();
    void pfk();
    void pdr();
    void pgt();
    void pdi();
    void enw();
    void eht();
    void ebo();
    void edi();
    void welcome();

  private:
    std::unique_ptr<Map> map_;
    std::shared_ptr<Client> client;
    ZappyGameObject::Map gameObjects;
    ZappyGameObject::Map ressources_;
    std::vector<Trantorian> trantorians_;
    std::vector<std::string> splitedBuffer_;
    std::shared_ptr<ZappyGui> zappyGui;
    std::unordered_map<std::string, glm::vec3> teamsColors_;
    std::unordered_map<std::string, FunctionPtr> _pointerToFunction; // Map of pointers to functions
};
} // namespace zappy
