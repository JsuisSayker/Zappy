/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** SimpleRenderSystem
*/

#include "SimpleRenderSystem.hpp"

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
#include <iostream>

namespace zappy {

struct SimplePushConstantData {
    glm::mat4 modelMatrix{1.f};
    glm::mat4 normalMatrix{1.f};
};

SimpleRenderSystem::SimpleRenderSystem(ZappyDevice &device,
    VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout,
    std::string executablePath)
    : lveDevice{device}, executablePath{executablePath}
{
    createPipelineLayout(globalSetLayout);
    createPipeline(renderPass);
}

SimpleRenderSystem::~SimpleRenderSystem()
{
    vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);
}

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
    if (vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo,
            nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void SimpleRenderSystem::createPipeline(VkRenderPass renderPass)
{
    assert(pipelineLayout != nullptr &&
        "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    ZappyPipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = pipelineLayout;
    lvePipeline = std::make_unique<ZappyPipeline>(lveDevice,
        executablePath + "/ZappyGui/shaders/SimpleShader.vert.spv",
        executablePath + "/ZappyGui/shaders/SimpleShader.frag.spv",
        pipelineConfig);
}

void SimpleRenderSystem::renderGameObjects(FrameInfo &frameInfo)
{
    lvePipeline->bind(frameInfo.commandBuffer);


    for (auto &kv : frameInfo.gameObjects) {
        auto &obj = kv.second;
        if (obj.model == nullptr)
            continue;

        if (obj.hasDescriptorSet) {
            vkCmdBindDescriptorSets(frameInfo.commandBuffer,
                VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1,
                &obj.descriptorSets[frameInfo.frameIndex], 0, nullptr);
            std::cout << "Binding descriptor set" << std::endl;
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
