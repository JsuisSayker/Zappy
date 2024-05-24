/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Zappy
*/

#include "Zappy.hpp"

#include <array>
#include <stdexcept>

/**
 * @brief Default constructor for the Zappy class.
 *
 * This constructor initializes the Zappy object by calling the following
 * functions:
 * - createPipelineLayout()
 * - createPipeline()
 * - createCommandBuffers()
 */
lve::Zappy::Zappy()
{
    createPipelineLayout();
    createPipeline();
    createCommandBuffers();
}

/**
 * @brief Destructor for the lve::Zappy class.
 *
 * This destructor is responsible for destroying the Vulkan pipeline layout
 * used by the Zappy class. It calls `vkDestroyPipelineLayout` to destroy the
 * pipeline layout object.
 *
 * @note The pipeline layout must be created and initialized before calling
 * this destructor.
 *
 * @see lve::Zappy
 */
lve::Zappy::~Zappy()
{
    vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);
}

void lve::Zappy::run()
{
    while (!lveWindow.shouldClose()) {
        glfwPollEvents();
        drawFrame();
    }
    vkDeviceWaitIdle(lveDevice.device());
}

/**
 * @brief Creates a pipeline layout.
 *
 * This function creates a Vulkan pipeline layout using the provided
 * information. It initializes a VkPipelineLayoutCreateInfo structure and calls
 * vkCreatePipelineLayout to create the pipeline layout. If the creation fails,
 * a std::runtime_error is thrown.
 */
void lve::Zappy::createPipelineLayout()
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo,
            nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

/**
 * @brief Creates the pipeline for rendering in Zappy.
 *
 * This function creates the pipeline configuration for rendering in the Zappy
 * application. It uses the default pipeline configuration provided by the
 * LvePipeline class and sets the render pass to the render pass obtained from
 * the LveSwapChain object.
 */
void lve::Zappy::createPipeline()
{
    auto pipelineConfig = lve::LvePipeline::defaultPipelineConfigInfo(
        lveSwapChain.width(), lveSwapChain.height());
    pipelineConfig.renderPass = lveSwapChain.getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;
    lvePipeline = std::make_unique<lve::LvePipeline>(lveDevice,
        "ZappyGUI/shaders/simple_shader.vert.spv",
        "ZappyGUI/shaders/simple_shader.frag.spv", pipelineConfig);
}

/**
 * @brief Creates the command buffers used for rendering.
 *
 * This function creates and allocates the command buffers used for recording rendering commands.
 * It allocates one command buffer per swap chain image, and records the necessary commands for rendering.
 * The command buffers are stored in the `commandBuffers` vector of the `lve::Zappy` class.
 *
 * @throws std::runtime_error if the command buffers fail to allocate or record commands.
 */
void lve::Zappy::createCommandBuffers()
{
    commandBuffers.resize(lveSwapChain.imageCount());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = lveDevice.getCommandPool();
    allocInfo.commandBufferCount =
        static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(lveDevice.device(), &allocInfo,
            commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }

    for (size_t i = 0; i < commandBuffers.size(); i++) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) !=
            VK_SUCCESS) {
            throw std::runtime_error(
                "failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = lveSwapChain.getRenderPass();
        renderPassInfo.framebuffer = lveSwapChain.getFrameBuffer(i);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = lveSwapChain.getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount =
            static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(
            commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        lvePipeline->bind(commandBuffers[i]);
        vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

        vkCmdEndRenderPass(commandBuffers[i]);

        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }
}

void lve::Zappy::drawFrame() 
{
    uint32_t imageIndex;
    auto result = lveSwapChain.acquireNextImage(&imageIndex);

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire next image!");
    }

    result = lveSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to submit command buffer!");
    }
}
