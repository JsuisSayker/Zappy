/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Lve_pipeline
*/

#pragma once

#include "Lve_device.hpp"

#include <string>
#include <vector>

namespace lve
{
    struct PipelineConfigInfo {
        VkViewport viewport; // The viewport for the pipeline
        VkRect2D scissor;   // The scissor for the pipeline
        VkPipelineViewportStateCreateInfo viewportInfo; // The viewport information for the pipeline
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo; // The input assembly information for the pipeline
        VkPipelineRasterizationStateCreateInfo rasterizationInfo; // The rasterization information for the pipeline
        VkPipelineMultisampleStateCreateInfo multisampleInfo; // The multisample information for the pipeline
        VkPipelineColorBlendAttachmentState colorBlendAttachment; // The color blend attachment for the pipeline
        VkPipelineColorBlendStateCreateInfo colorBlendInfo; // The color blend information for the pipeline
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo; // The depth stencil information for the pipeline
        VkPipelineLayout pipelineLayout = nullptr; // The pipeline layout for the pipeline
        VkRenderPass renderPass = nullptr; // The render pass for the pipeline
        uint32_t subpass = 0; // The subpass for the pipeline
    };

    class LvePipeline {
        public:
            LvePipeline(LveDevice &device, const  std::string &vertFilepath, const std::string &fragFilepath, const PipelineConfigInfo &configInfo);
            ~LvePipeline();

            LvePipeline(const LvePipeline &) = delete;
            void operator=(const LvePipeline &) = delete;

            static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

        private:
            static std::vector<char> readFile(const std::string &filepath);
            void createGraphicsPipeline(const std::string &vertFilepath, const std::string &fragFilepath, const PipelineConfigInfo &configInfo);
            void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

            LveDevice &device;
            VkPipeline graphicsPipeline;
            VkShaderModule vertShaderModule;
            VkShaderModule fragShaderModule;
    };
}
