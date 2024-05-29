#pragma once

#include "../camera.hpp"
#include "../device.hpp"
#include "../frame_info.hpp"
#include "../game_object.hpp"
#include "../pipeline.hpp"

// std
#include <memory>
#include <vector>

namespace zappy {
class SimpleRenderSystem {
 public:
  SimpleRenderSystem(
      ZappyDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout, std::string executablePath);
  ~SimpleRenderSystem();

  SimpleRenderSystem(const SimpleRenderSystem &) = delete;
  SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

  void renderGameObjects(FrameInfo &frameInfo);

 private:
  void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
  void createPipeline(VkRenderPass renderPass);

  ZappyDevice &lveDevice;

  std::unique_ptr<ZappyPipeline> lvePipeline;
  VkPipelineLayout pipelineLayout;
  std::string executablePath;
};
}  // namespace zappy
