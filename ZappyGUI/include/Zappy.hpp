/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Zappy
*/

#pragma once

#include "Lve_pipeline.hpp"
#include "Lve_window.hpp"
#include "Lve_device.hpp"
#include "Lve_swap_chain.hpp"

#include <memory>
#include <vector>

namespace lve {
class Zappy {
  public:
    Zappy();
    ~Zappy();

    Zappy(const Zappy &) = delete;
    void operator=(const Zappy &) = delete;

    const int height = 800; // The height of the window
    const int width = 600;  // The width of the window

    void run();

  private:

    void createPipelineLayout();
    void createPipeline();
    void createCommandBuffers();
    void drawFrame();

    LveWindow lveWindow{width, width, "Zappy"}; // The window object
    LveDevice lveDevice{lveWindow};             // The device object
    LveSwapChain lveSwapChain{lveDevice, lveWindow.getExtent()}; // The swap chain object
    std::unique_ptr<LvePipeline> lvePipeline; // The pipeline object
    VkPipelineLayout pipelineLayout;           // The pipeline layout
    std::vector<VkCommandBuffer> commandBuffers;         // The pipelines
};
} // namespace lve
