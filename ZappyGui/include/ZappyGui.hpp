#pragma once

#include "Descriptors.hpp"
#include "Device.hpp"
#include "GameObject.hpp"
#include "Renderer.hpp"
#include "Window.hpp"
#include "Texture.hpp"

// std
#include <memory>
#include <vector>

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
};
}  // namespace zappy
