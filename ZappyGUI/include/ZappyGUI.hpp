#pragma once

#include "Descriptors.hpp"
#include "Device.hpp"
#include "GameObject.hpp"
#include "Renderer.hpp"
#include "Window.hpp"

// std
#include <memory>
#include <vector>

namespace zappy {
  class ZappyGui {
   public:
    static constexpr int WIDTH = 1920;
    static constexpr int HEIGHT = 1080;
  
    ZappyGui(int width, int height);
    ~ZappyGui();
  
    ZappyGui(const ZappyGui &) = delete;
    ZappyGui &operator=(const ZappyGui &) = delete;
  
    void run();
  
   private:
    void loadMap(int width, int height);
  
    ZappyWindow lveWindow{WIDTH, HEIGHT, "Vulkan Tutorial"};
    ZappyDevice lveDevice{lveWindow};
    ZappyRenderer lveRenderer{lveWindow, lveDevice};
  
    // note: order of declarations matters
    std::unique_ptr<LveDescriptorPool> globalPool{};
    ZappyGameObject::Map gameObjects;
  };
}  // namespace zappy
