/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Pipeline
*/

#pragma once

#include "Device.hpp"

// std
#include <string>
#include <vector>

namespace zappy {

struct PipelineConfigInfo {
    PipelineConfigInfo() = default;
    PipelineConfigInfo(const PipelineConfigInfo &) = delete;
    PipelineConfigInfo &operator=(const PipelineConfigInfo &) = delete;

    std::vector<VkVertexInputBindingDescription> bindingDescriptions{};
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
    VkPipelineViewportStateCreateInfo viewportInfo;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    std::vector<VkDynamicState> dynamicStateEnables;
    VkPipelineDynamicStateCreateInfo dynamicStateInfo;
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;
};

class ZappyPipeline {
  public:
    ZappyPipeline(ZappyDevice &device, const std::string &vertFilepath,
        const std::string &fragFilepath, const PipelineConfigInfo &configInfo);
    ~ZappyPipeline();

    ZappyPipeline(const ZappyPipeline &) = delete;
    ZappyPipeline &operator=(const ZappyPipeline &) = delete;

    void bind(VkCommandBuffer commandBuffer);

    static void defaultPipelineConfigInfo(PipelineConfigInfo &configInfo);
    static void enableAlphaBlending(PipelineConfigInfo &configInfo);

  private:
    static std::vector<char> readFile(const std::string &filepath);

    void createGraphicsPipeline(const std::string &vertFilepath,
        const std::string &fragFilepath, const PipelineConfigInfo &configInfo);

    void createShaderModule(
        const std::vector<char> &code, VkShaderModule *shaderModule);

    ZappyDevice &zappyDevice;
    VkPipeline graphicsPipeline;
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
};
} // namespace zappy
