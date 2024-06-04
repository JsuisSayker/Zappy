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
class ZappyGui {
  public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    ZappyGui();
    ~ZappyGui();

    ZappyGui(const ZappyGui &) = delete;
    ZappyGui &operator=(const ZappyGui &) = delete;

    static std::string getExecutablePath();
    std::unique_ptr<GameContent> &getGameContent();

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

    std::string executablePath;

    std::unique_ptr<GameContent> gameContent;
};
} // namespace zappy
