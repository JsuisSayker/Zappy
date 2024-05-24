#pragma once

#include "camera.hpp"
#include "device.hpp"
#include "frame_info.hpp"
#include "game_object.hpp"
#include "pipeline.hpp"

// std
#include <memory>
#include <vector>

namespace zappy {
class RenderSystem {
 public:
  RenderSystem(
      ZappyDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
  ~RenderSystem();

  RenderSystem(const RenderSystem &) = delete;
  RenderSystem &operator=(const RenderSystem &) = delete;

  void renderGameObjects(FrameInfo &frameInfo);

 private:
  void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
  void createPipeline(VkRenderPass renderPass);

  ZappyDevice &lveDevice;

  std::unique_ptr<ZappyPipeline> lvePipeline;
  VkPipelineLayout pipelineLayout;
};
}  // namespace zappy
