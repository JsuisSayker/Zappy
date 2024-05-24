#pragma once

#include "descriptors.hpp"
#include "device.hpp"
#include "game_object.hpp"
#include "renderer.hpp"
#include "window.hpp"

// std
#include <memory>
#include <vector>

namespace zappy {
class ZappyGui {
 public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  ZappyGui();
  ~ZappyGui();

  ZappyGui(const ZappyGui &) = delete;
  ZappyGui &operator=(const ZappyGui &) = delete;

  void run();

 private:
  void loadGameObjects();

  ZappyWindow lveWindow{WIDTH, HEIGHT, "Vulkan Tutorial"};
  ZappyDevice lveDevice{lveWindow};
  ZappyRenderer lveRenderer{lveWindow, lveDevice};

  // note: order of declarations matters
  std::unique_ptr<LveDescriptorPool> globalPool{};
  ZappyGameObject::Map gameObjects;
};
}  // namespace zappy
