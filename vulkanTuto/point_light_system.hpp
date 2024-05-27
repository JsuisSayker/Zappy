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
class PointLightSystem {
 public:
  PointLightSystem(
      ZappyDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
  ~PointLightSystem();

  PointLightSystem(const PointLightSystem &) = delete;
  PointLightSystem &operator=(const PointLightSystem &) = delete;

  void update(FrameInfo &frameInfo, GlobalUbo &ubo);
  void render(FrameInfo &frameInfo);

 private:
  void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
  void createPipeline(VkRenderPass renderPass);

  ZappyDevice &lveDevice;

  std::unique_ptr<ZappyPipeline> lvePipeline;
  VkPipelineLayout pipelineLayout;
};
}  // namespace zappy
