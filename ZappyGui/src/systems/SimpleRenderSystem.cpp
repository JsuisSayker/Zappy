/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** SimpleRenderSystem
*/

#include "SimpleRenderSystem.hpp"
#include "ErrorHandling.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <filesystem>
#include <stdexcept>

/**
 * @file SimpleRenderSystem.cpp
 * @brief Implementation of the SimpleRenderSystem class.
 */

namespace zappy {

/**
 * @struct SimplePushConstantData
 * @brief Structure representing the push constant data used in the SimpleRenderSystem.
 */
struct SimplePushConstantData {
    glm::mat4 modelMatrix{1.f}; /**< Model matrix */
    glm::mat4 normalMatrix{1.f}; /**< Normal matrix */
};

/**
 * @brief Constructs a SimpleRenderSystem object.
 * @param device The ZappyDevice object.
 * @param renderPass The Vulkan render pass.
 * @param globalSetLayout The descriptor set layout.
 * @param executablePath The path to the executable.
 */
SimpleRenderSystem::SimpleRenderSystem(ZappyDevice &device,
    VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout,
    std::string executablePath)
    : zappyDevice{device}, executablePath{executablePath}
{
    createPipelineLayout(globalSetLayout);
    createPipeline(renderPass);
}

/**
 * @brief Destroys the SimpleRenderSystem object.
 */
SimpleRenderSystem::~SimpleRenderSystem()
{
    vkDestroyPipelineLayout(zappyDevice.device(), pipelineLayout, nullptr);
}

/**
 * @brief Creates the pipeline layout.
 * @param globalSetLayout The descriptor set layout.
 */
void SimpleRenderSystem::createPipelineLayout(
    VkDescriptorSetLayout globalSetLayout)
{
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags =
        VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);

    std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout};

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount =
        static_cast<uint32_t>(descriptorSetLayouts.size());
    pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    if (vkCreatePipelineLayout(zappyDevice.device(), &pipelineLayoutInfo,
            nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw zappy::PipelineLayoutCreationFailedException();
    }
}

/**
 * @brief Creates the pipeline.
 * @param renderPass The Vulkan render pass.
 */
void SimpleRenderSystem::createPipeline(VkRenderPass renderPass)
{
    assert(pipelineLayout != nullptr &&
        "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    ZappyPipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = pipelineLayout;
    zappyPipeline = std::make_unique<ZappyPipeline>(zappyDevice,
        executablePath + "/ZappyGui/shaders/SimpleShader.vert.spv",
        executablePath + "/ZappyGui/shaders/SimpleShader.frag.spv",
        pipelineConfig);
}

/**
 * @brief Renders the game objects.
 * @param frameInfo The frame information.
 */
void SimpleRenderSystem::renderGameObjects(FrameInfo &frameInfo)
{
    zappyPipeline->bind(frameInfo.commandBuffer);

    int frameIndex = frameInfo.frameIndex;
    for (auto &kv : frameInfo.gameObjects) {
        auto &obj = kv.second;
        if (obj.model == nullptr)
            continue;

        if (obj.hasDescriptorSet) {
            vkCmdBindDescriptorSets(frameInfo.commandBuffer,
                VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1,
                &frameInfo.textureObjects[obj.indexDescriptorSet]
                     .first[frameIndex],
                0, nullptr);
        } else {
            vkCmdBindDescriptorSets(frameInfo.commandBuffer,
                VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1,
                &frameInfo.globalDescriptorSet, 0, nullptr);
        }
        SimplePushConstantData push{};
        push.modelMatrix = obj.transform.mat4();
        push.normalMatrix = obj.transform.normalMatrix();

        vkCmdPushConstants(frameInfo.commandBuffer, pipelineLayout,
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
            sizeof(SimplePushConstantData), &push);
        obj.model->bind(frameInfo.commandBuffer);
        obj.model->draw(frameInfo.commandBuffer);
    }
}

} // namespace zappy
