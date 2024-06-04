/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ZappyGui
*/

#pragma once

#include "Client.hpp"
#include "Descriptors.hpp"
#include "Device.hpp"
#include "GameObject.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include "Window.hpp"
#include "GameContent.hpp"

// std
#include <memory>
#include <vector>
#include <signal.h>

namespace zappy {
class FirstApp {
  public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    FirstApp();
    ~FirstApp();

    FirstApp(const FirstApp &) = delete;
    FirstApp &operator=(const FirstApp &) = delete;

    static std::string getExecutablePath();

    void setClient(std::shared_ptr<Client> client) { this->client = client; }
    std::shared_ptr<Client> getClient() { return this->client; }

    void run();

  private:
    void loadGameObjects();

    ZappyWindow lveWindow{WIDTH, HEIGHT, "Vulkan Tutorial"};
    ZappyDevice lveDevice{lveWindow};
    ZappyRenderer lveRenderer{lveWindow, lveDevice};

    // note: order of declarations matters
    std::unique_ptr<ZappyDescriptorPool> globalPool{};
    std::unique_ptr<Texture> texture{};
    ZappyGameObject::Map gameObjects;

    std::shared_ptr<Client> client;
    std::string executablePath;

    GameContent gameContent;
};
} // namespace zappy
