/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** SimpleRenderSystem
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
class SimpleRenderSystem {
  public:
    SimpleRenderSystem(ZappyDevice &device, VkRenderPass renderPass,
        VkDescriptorSetLayout globalSetLayout, std::string executablePath);
    ~SimpleRenderSystem();

    SimpleRenderSystem(const SimpleRenderSystem &) = delete;
    SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

    void renderGameObjects(FrameInfo &frameInfo);

  private:
    void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
    void createPipeline(VkRenderPass renderPass);

    ZappyDevice &zappyDevice;

    std::unique_ptr<ZappyPipeline> zappyPipeline;
    VkPipelineLayout pipelineLayout;
    std::string executablePath;
};
} // namespace zappy
