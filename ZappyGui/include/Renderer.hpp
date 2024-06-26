/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Renderer
*/

#pragma once

#include "Device.hpp"
#include "SwapChain.hpp"
#include "Window.hpp"

// std
#include <cassert>
#include <memory>
#include <vector>

namespace zappy {
class ZappyRenderer {
  public:
    ZappyRenderer(ZappyWindow &window, ZappyDevice &device);
    ~ZappyRenderer();

    ZappyRenderer(const ZappyRenderer &) = delete;
    ZappyRenderer &operator=(const ZappyRenderer &) = delete;

    VkRenderPass getSwapChainRenderPass() const
    {
        return zappySwapChain->getRenderPass();
    }
    float getAspectRatio() const { return zappySwapChain->extentAspectRatio(); }
    bool isFrameInProgress() const { return isFrameStarted; }

    VkCommandBuffer getCurrentCommandBuffer() const
    {
        assert(isFrameStarted &&
            "Cannot get command buffer when frame not in progress");
        return commandBuffers[currentFrameIndex];
    }

    int getFrameIndex() const
    {
        assert(isFrameStarted &&
            "Cannot get frame index when frame not in progress");
        return currentFrameIndex;
    }

    VkCommandBuffer beginFrame();
    void endFrame();
    void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
    void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

    std::unique_ptr<ZappySwapChain> &getSwapChain() { return zappySwapChain; }

  private:
    void createCommandBuffers();
    void freeCommandBuffers();
    void recreateSwapChain();

    ZappyWindow &zappyWindow;
    ZappyDevice &zappyDevice;
    std::unique_ptr<ZappySwapChain> zappySwapChain;
    std::vector<VkCommandBuffer> commandBuffers;

    uint32_t currentImageIndex;
    int currentFrameIndex{0};
    bool isFrameStarted{false};
};
} // namespace zappy
