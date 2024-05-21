/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** first_app
*/

#pragma once

#include "lve_window.hpp"
#include "lve_device.hpp"
#include "lve_pipeline.hpp"
#include "lve_swap_chain.hpp"

#include <memory>
#include <vector>

namespace lve {
    class FirstApp {
        public:
            static const int WIDTH = 800;
            static const int HEIGHT = 600;
            
            FirstApp();
            ~FirstApp();

            FirstApp(const FirstApp &) = delete;
            FirstApp &operator=(const FirstApp &) = delete;

            void run();

        private:
            void createPipelineLayout();
            void createPipeline();
            void createCommandBuffers();
            void drawFrame();

            LveWindow lveWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
            LveDevice lveDevice{lveWindow};
            LveSwapChain lveSwapChain{lveDevice, lveWindow.getExtent()};
            std::unique_ptr<LvePipeline> lvePipeline;
            VkPipelineLayout pipelineLayout;
            std::vector<VkCommandBuffer> commandBuffers;
    };
}