/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** PointLightSystem
*/

#pragma once

#include "../../include/Camera.hpp"
#include "../../include/Device.hpp"
#include "../../include/FrameInfo.hpp"
#include "../../include/GameObject.hpp"
#include "../../include/Pipeline.hpp"

// std
#include <memory>
#include <vector>

namespace zappy {
class PointLightSystem {
  public:
    PointLightSystem(ZappyDevice &device, VkRenderPass renderPass,
        VkDescriptorSetLayout globalSetLayout, std::string executablePath);
    ~PointLightSystem();

    PointLightSystem(const PointLightSystem &) = delete;
    PointLightSystem &operator=(const PointLightSystem &) = delete;

    void update(FrameInfo &frameInfo, GlobalUbo &ubo);
    void render(FrameInfo &frameInfo);

  private:
    void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
    void createPipeline(VkRenderPass renderPass);

    ZappyDevice &zappyDevice;

    std::unique_ptr<ZappyPipeline> zappyPipeline;
    VkPipelineLayout pipelineLayout;
    std::string executablePath;
};
} // namespace zappy
