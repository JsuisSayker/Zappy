/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Renderer
*/

#include "Renderer.hpp"
#include "ErrorHandling.hpp"

// std
#include <array>
#include <cassert>
#include <stdexcept>

namespace zappy {


/**
 * @brief Constructs a ZappyRenderer object.
 * 
 * @param window The ZappyWindow object used for rendering.
 * @param device The ZappyDevice object used for rendering.
 */
ZappyRenderer::ZappyRenderer(ZappyWindow &window, ZappyDevice &device)
    : zappyWindow{window}, zappyDevice{device}
{
    recreateSwapChain();
    createCommandBuffers();
}

/**
 * @brief Destroys the ZappyRenderer object.
 * 
 */
ZappyRenderer::~ZappyRenderer() { freeCommandBuffers(); }

/**
 * @brief Recreates the swap chain.
 * 
 */
void ZappyRenderer::recreateSwapChain()
{
    auto extent = zappyWindow.getExtent();
    while (extent.width == 0 || extent.height == 0) {
        extent = zappyWindow.getExtent();
        glfwWaitEvents();
    }
    vkDeviceWaitIdle(zappyDevice.device());

    if (zappySwapChain == nullptr) {
        zappySwapChain = std::make_unique<ZappySwapChain>(zappyDevice, extent);
    } else {
        std::shared_ptr<ZappySwapChain> oldSwapChain = std::move(zappySwapChain);
        zappySwapChain =
            std::make_unique<ZappySwapChain>(zappyDevice, extent, oldSwapChain);

        if (!oldSwapChain->compareSwapFormats(*zappySwapChain.get())) {
            throw zappy::SwapChainFormatChangedException();
        }
    }
}

/**
 * @brief Creates the command buffers.
 * 
 */
void ZappyRenderer::createCommandBuffers()
{
    commandBuffers.resize(ZappySwapChain::MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = zappyDevice.getCommandPool();
    allocInfo.commandBufferCount =
        static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(zappyDevice.device(), &allocInfo,
            commandBuffers.data()) != VK_SUCCESS) {
        throw zappy::MemoryAllocationFailedException();
    }
}

/**
 * @brief Frees the command buffers.
 * 
 */
void ZappyRenderer::freeCommandBuffers()
{
    vkFreeCommandBuffers(zappyDevice.device(), zappyDevice.getCommandPool(),
        static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
    commandBuffers.clear();
}

/**
 * @brief Gets the current command buffer.
 * 
 * @return VkCommandBuffer The current command buffer.
 */
VkCommandBuffer ZappyRenderer::beginFrame()
{
    assert(
        !isFrameStarted && "Can't call beginFrame while already in progress");

    auto result = zappySwapChain->acquireNextImage(&currentImageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return nullptr;
    }

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw zappy::SwapChainImageAdquisitionFailedException();
    }

    isFrameStarted = true;

    auto commandBuffer = getCurrentCommandBuffer();
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw zappy::CommandBufferRecordingFailedToBeginException();
    }

    return commandBuffer;
}

/**
 * @brief Ends the frame.
 * 
 */
void ZappyRenderer::endFrame()
{
    assert(isFrameStarted &&
        "Can't call endFrame while frame is not in progress");
    auto commandBuffer = getCurrentCommandBuffer();
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw zappy::CommandBufferRecordingFailedException();
    }

    auto result =
        zappySwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
        zappyWindow.wasWindowResized()) {
        zappyWindow.resetWindowResizedFlag();
        recreateSwapChain();
    } else if (result != VK_SUCCESS) {
        throw zappy::SwapChainImagePresentationFailedException();
    }

    isFrameStarted = false;
    currentFrameIndex =
        (currentFrameIndex + 1) % ZappySwapChain::MAX_FRAMES_IN_FLIGHT;
}

/**
 * @brief Begins the swap chain render pass.
 * 
 * @param commandBuffer The command buffer.
 */
void ZappyRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
    assert(isFrameStarted &&
        "Can't call beginSwapChainRenderPass if frame is not in progress");
    assert(commandBuffer == getCurrentCommandBuffer() &&
        "Can't begin render pass on command buffer from a different frame");

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = zappySwapChain->getRenderPass();
    renderPassInfo.framebuffer =
        zappySwapChain->getFrameBuffer(currentImageIndex);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = zappySwapChain->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(
        commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width =
        static_cast<float>(zappySwapChain->getSwapChainExtent().width);
    viewport.height =
        static_cast<float>(zappySwapChain->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0, 0}, zappySwapChain->getSwapChainExtent()};
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}


/**
 * Ends the current swap chain render pass on the specified command buffer.
 * 
 * @param commandBuffer The command buffer to end the render pass on.
 * @throws std::runtime_error if the frame is not in progress or if the command buffer
 *         is from a different frame.
 */
void ZappyRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
    assert(isFrameStarted &&
        "Can't call endSwapChainRenderPass if frame is not in progress");
    assert(commandBuffer == getCurrentCommandBuffer() &&
        "Can't end render pass on command buffer from a different frame");
    vkCmdEndRenderPass(commandBuffer);
}

} // namespace zappy
