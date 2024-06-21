/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** PointLightSystem
*/

#include "PointLightSystem.hpp"
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
#include <map>
#include <stdexcept>

/**
 * @file PointLightSystem.cpp
 * @brief Implementation of the PointLightSystem class.
 */

namespace zappy {

/**
 * @struct PointLightPushConstants
 * @brief Structure representing the push constants for the point light.
 */
struct PointLightPushConstants {
    glm::vec4 position{}; /**< The position of the point light. */
    glm::vec4 color{}; /**< The color of the point light. */
    float radius; /**< The radius of the point light. */
};

/**
 * @class PointLightSystem
 * @brief Class responsible for managing point lights in the scene.
 */
PointLightSystem::PointLightSystem(ZappyDevice &device,
    VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout,
    std::string executablePath)
    : lveDevice{device}, executablePath{executablePath}
{
    createPipelineLayout(globalSetLayout);
    createPipeline(renderPass);
}

/**
 * @brief Destructor for the PointLightSystem class.
 */
PointLightSystem::~PointLightSystem()
{
    vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);
}

/**
 * @brief Creates the pipeline layout for the point light system.
 * @param globalSetLayout The descriptor set layout for the global uniform buffer object.
 */
void PointLightSystem::createPipelineLayout(
    VkDescriptorSetLayout globalSetLayout)
{
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags =
        VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(PointLightPushConstants);

    std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout};

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
    pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    if (vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
        VK_SUCCESS) {
        throw zappy::PipelineLayoutCreationFailedException();
    }
}

/**
 * @brief Creates the pipeline for the point light system.
 * @param renderPass The render pass to use for the pipeline.
 */
void PointLightSystem::createPipeline(VkRenderPass renderPass)
{
    assert(pipelineLayout != nullptr &&
        "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    ZappyPipeline::defaultPipelineConfigInfo(pipelineConfig);
    ZappyPipeline::enableAlphaBlending(pipelineConfig);
    pipelineConfig.attributeDescriptions.clear();
    pipelineConfig.bindingDescriptions.clear();
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = pipelineLayout;
    lvePipeline = std::make_unique<ZappyPipeline>(lveDevice,
        executablePath + "/ZappyGui/shaders/PointLight.vert.spv",
        executablePath + "/ZappyGui/shaders/PointLight.frag.spv",
        pipelineConfig);
}

/**
 * @brief Updates the point lights in the scene.
 * @param frameInfo The frame information.
 * @param ubo The global uniform buffer object.
 */
void PointLightSystem::update(FrameInfo &frameInfo, GlobalUbo &ubo)
{
    int lightIndex = 0;
    for (auto &kv : frameInfo.gameObjects) {
        auto &obj = kv.second;
        if (obj.pointLight == nullptr)
            continue;

        assert(lightIndex < MAX_LIGHTS &&
            "Point lights exceed maximum specified");

        // copy light to ubo
        ubo.pointLights[lightIndex].position =
            glm::vec4(obj.transform.translation, 1.f);
        ubo.pointLights[lightIndex].color =
            glm::vec4(obj.color, obj.pointLight->lightIntensity);

        lightIndex += 1;
    }
    ubo.numLights = lightIndex;
}

/**
 * @brief Renders the point lights in the scene.
 * @param frameInfo The frame information.
 */
void PointLightSystem::render(FrameInfo &frameInfo)
{
    // sort lights
    std::map<float, ZappyGameObject::id_t> sorted;
    for (auto &kv : frameInfo.gameObjects) {
        auto &obj = kv.second;
        if (obj.pointLight == nullptr)
            continue;

        // calculate distance
        auto offset =
            frameInfo.camera.getPosition() - obj.transform.translation;
        float disSquared = glm::dot(offset, offset);
        sorted[disSquared] = obj.getId();
    }

    lvePipeline->bind(frameInfo.commandBuffer);

    vkCmdBindDescriptorSets(frameInfo.commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1,
        &frameInfo.globalDescriptorSet, 0, nullptr);

    // iterate through sorted lights in reverse order
    for (auto it = sorted.rbegin(); it != sorted.rend(); ++it) {
        // use game obj id to find light object
        auto &obj = frameInfo.gameObjects.at(it->second);

        PointLightPushConstants push{};
        push.position = glm::vec4(obj.transform.translation, 1.f);
        push.color = glm::vec4(obj.color, obj.pointLight->lightIntensity);
        push.radius = obj.transform.scale.x;

        vkCmdPushConstants(frameInfo.commandBuffer, pipelineLayout,
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
            sizeof(PointLightPushConstants), &push);
        vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
    }
}

} // namespace zappy
