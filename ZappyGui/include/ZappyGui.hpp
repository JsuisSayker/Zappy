/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ZappyGui
*/

#pragma once

#include <vulkan/vulkan_core.h>

#include "Client.hpp"
#include "Descriptors.hpp"
#include "Device.hpp"
#include "GameObject.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include "Window.hpp"
#include "Trantorian.hpp"
#include "Camera.hpp"

// std
#include <memory>
#include <signal.h>
#include <vector>

namespace zappy {
class ZappyGui {
  public:
    using FunctionPtr = std::function<void(std::vector<std::string>)>;
    static constexpr int WIDTH = 1920;
    static constexpr int HEIGHT = 1080;

    ZappyGui();
    ~ZappyGui();

    ZappyGui(const ZappyGui &) = delete;
    ZappyGui &operator=(const ZappyGui &) = delete;

    static std::string getExecutablePath();

    void addTrantorian(const std::string &teamName, const glm::vec3 &position, int playerNumber, int orientation);
    void removeTrantorian(int playerNumber);
    void updateTrantorianPosition(int playerNumber, const glm::vec3 &position, int orientation);

    void setPointerToFunction(std::unordered_map<std::string, FunctionPtr> &pointerToFunction);

    std::unordered_map<std::string, FunctionPtr> &getPointerToFunction();

    std::vector<std::vector<std::string>> &getSplitBuffer();

    void bufferToSplitBuffer(std::string buffer);

    void setClient(std::shared_ptr<Client> client) { this->client = client; }

    std::shared_ptr<Client> getClient() { return this->client; }

    // Functions to handle commands
    void msz(std::vector<std::string> actualCommand);
    void bct(std::vector<std::string> actualCommand);
    void mct(std::vector<std::string> actualCommand);
    void tna(std::vector<std::string> actualCommand);
    void pnw(std::vector<std::string> actualCommand);
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

    void run();

    // ImGUI Functions
    void initImGui();
    void drawGui();

  protected:
    void createMap(int width, int height);
    void loadGameObjects();
    ZappyGameObject::id_t createGameObject(const std::string &modelPath,
        const std::string &texturePath, const glm::vec3 &position,
        const glm::vec3 &rotation, const glm::vec3 &scale, bool hasTexture);
    void removeGameObject(ZappyGameObject::id_t gameObjectId);

    ZappyWindow lveWindow{WIDTH, HEIGHT, "Vulkan Tutorial"};
    ZappyDevice lveDevice{lveWindow};
    ZappyRenderer lveRenderer{lveWindow, lveDevice};

    // note: order of declarations matters
    std::unique_ptr<ZappyDescriptorPool> globalPool{};
    std::unique_ptr<Texture> texture{};
    ZappyGameObject::Map gameObjects;

    std::string executablePath;
    ZappyCamera camera;
    ZappyGameObject viewerObject{ZappyGameObject::createGameObject()};
    std::vector<std::unique_ptr<ZappyBuffer>> uboBuffers;
    std::unique_ptr<ZappyDescriptorSetLayout> globalSetLayout;
    std::vector<std::pair<std::shared_ptr<ZappyModel>, std::string>> modelObjects;
    std::vector<std::pair<std::vector<VkDescriptorSet>, std::pair<std::shared_ptr<Texture>, std::string>>> textureObjects;
    std::unique_ptr<Map> map_;
    std::shared_ptr<Client> client;
    ZappyGameObject::Map ressources_;
    std::vector<Trantorian> trantorians_;
    std::unordered_map<std::string, glm::vec3> teamsColors_;
    std::unordered_map<std::string, FunctionPtr> _pointerToFunction; // Map of pointers to functions
    int _timeUnit; // Time unit of the server
};
} // namespace zappy
