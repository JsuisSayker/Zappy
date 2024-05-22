/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Zappy
*/

#include "Zappy.hpp"

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
    }
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

void lve::Zappy::createCommandBuffers() {}

void lve::Zappy::drawFrame() {}
