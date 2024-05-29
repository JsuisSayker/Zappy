#include "zappy_gui.hpp"

#include "keyboard_movement_controller.hpp"
#include "buffer.hpp"
#include "camera.hpp"
#include "frame_info.hpp"
#include "point_light_system.hpp"
#include "render_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <chrono>
#include <stdexcept>
#include <iostream>

namespace zappy {

ZappyGui::ZappyGui(int width, int height) : mapWidth(width), mapHeight(height)
{
    globalPool = LveDescriptorPool::Builder(lveDevice)
                     .setMaxSets(ZappySwapChain::MAX_FRAMES_IN_FLIGHT)
                     .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                         ZappySwapChain::MAX_FRAMES_IN_FLIGHT)
                     .build();
    loadMap();
}

ZappyGui::~ZappyGui() {}

void ZappyGui::run()
{
    std::vector<std::unique_ptr<ZappyBuffer>> uboBuffers(
        ZappySwapChain::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < uboBuffers.size(); i++) {
        uboBuffers[i] = std::make_unique<ZappyBuffer>(lveDevice,
            sizeof(GlobalUbo), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        uboBuffers[i]->map();
    }

    auto globalSetLayout =
        ZappyDescriptorSetLayout::Builder(lveDevice)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                VK_SHADER_STAGE_ALL_GRAPHICS)
            .build();

    std::vector<VkDescriptorSet> globalDescriptorSets(
        ZappySwapChain::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < globalDescriptorSets.size(); i++) {
        auto bufferInfo = uboBuffers[i]->descriptorInfo();
        LveDescriptorWriter(*globalSetLayout, *globalPool)
            .writeBuffer(0, &bufferInfo)
            .build(globalDescriptorSets[i]);
    }

    RenderSystem simpleRenderSystem{lveDevice,
        lveRenderer.getSwapChainRenderPass(),
        globalSetLayout->getDescriptorSetLayout()};
    PointLightSystem pointLightSystem{lveDevice,
        lveRenderer.getSwapChainRenderPass(),
        globalSetLayout->getDescriptorSetLayout()};
    ZappyCamera camera{};

    auto viewerObject = ZappyGameObject::createGameObject();
    viewerObject.transform.translation.x = mapWidth / 2.f;
    viewerObject.transform.translation.y = -mapWidth / 1.5f;
    if (mapWidth < mapHeight) {
        viewerObject.transform.translation.y = -mapHeight / 1.5f;
    }
    viewerObject.transform.translation.z = mapHeight / 2.f;
    viewerObject.transform.rotation.x = -40.f;
    KeyboardMovementController cameraController{};

    auto currentTime = std::chrono::high_resolution_clock::now();
    while (!lveWindow.shouldClose()) {
        // print camera position
        // std::cout << "Camera position: " << viewerObject.transform.translation.x
                //   << ", " << viewerObject.transform.translation.y << ", "
                //   << viewerObject.transform.translation.z << std::endl;
        glfwPollEvents();

        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime =
            std::chrono::duration<float, std::chrono::seconds::period>(
                newTime - currentTime)
                .count();
        currentTime = newTime;

        cameraController.moveInPlaneXZ(
            lveWindow.getGLFWwindow(), frameTime, viewerObject);
        camera.setViewYXZ(viewerObject.transform.translation,
            viewerObject.transform.rotation);

        float aspect = lveRenderer.getAspectRatio();
        camera.setPerspectiveProjection(
            glm::radians(50.f), aspect, 0.1f, 100.f);

        if (auto commandBuffer = lveRenderer.beginFrame()) {
            int frameIndex = lveRenderer.getFrameIndex();
            FrameInfo frameInfo{frameIndex, frameTime, commandBuffer, camera,
                globalDescriptorSets[frameIndex], gameObjects};

            // update
            GlobalUbo ubo{};
            ubo.projection = camera.getProjection();
            ubo.view = camera.getView();
            ubo.inverseView = camera.getInverseView();
            pointLightSystem.update(frameInfo, ubo);
            uboBuffers[frameIndex]->writeToBuffer(&ubo);
            uboBuffers[frameIndex]->flush();

            // render
            lveRenderer.beginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.renderGameObjects(frameInfo);
            pointLightSystem.render(frameInfo);
            lveRenderer.endSwapChainRenderPass(commandBuffer);
            lveRenderer.endFrame();
        }
    }

    vkDeviceWaitIdle(lveDevice.device());
}

void ZappyGui::loadMap()
{
    std::shared_ptr<ZappyModel> whiteCubeModel =
        ZappyModel::createModelFromFile(lveDevice, "models/cube_white.obj");
    std::shared_ptr<ZappyModel> blackCubeModel =
        ZappyModel::createModelFromFile(lveDevice, "models/cube_black.obj");
    for (int x = 0; x < mapWidth; x++) {
        for (int z = 0; z < mapHeight; z++) {
            auto cube = ZappyGameObject::createGameObject();
            cube.model = (x + z) % 2 == 0 ? whiteCubeModel : blackCubeModel;
            cube.transform.translation = {x, 0.f, z};
            cube.transform.scale = {1.f, 1.f, 1.f};
            gameObjects.emplace(cube.getId(), std::move(cube));
        }
    }
}

} // namespace zappy