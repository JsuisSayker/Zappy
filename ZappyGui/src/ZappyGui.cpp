/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ZappyGui
*/

#include "ZappyGui.hpp"
#include <memory>
#include <vulkan/vulkan_core.h>

#include "Buffer.hpp"
#include "Camera.hpp"
#include "ErrorHandling.hpp"
#include "GameContent.hpp"
#include "KeyboardMovementController.hpp"
#include "Trantorian.hpp"
#include "systems/PointLightSystem.hpp"
#include "systems/SimpleRenderSystem.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <unistd.h>

namespace zappy {

ZappyGui::ZappyGui()
{
    this->gameContent = std::make_unique<GameContent>();
    executablePath = getExecutablePath();
    globalPool = ZappyDescriptorPool::Builder(lveDevice)
                     .setMaxSets(ZappySwapChain::MAX_FRAMES_IN_FLIGHT)
                     .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                         ZappySwapChain::MAX_FRAMES_IN_FLIGHT)
                     .addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                         ZappySwapChain::MAX_FRAMES_IN_FLIGHT)
                     .build();
    loadGameObjects();
}

ZappyGui::~ZappyGui() {}

void ZappyGui::run()
{
    this->gameContent.get()->getClient()->connectToServer();
    fd_set readfds;

    uboBuffers.resize(
        ZappySwapChain::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < uboBuffers.size(); i++) {
        uboBuffers[i] = std::make_unique<ZappyBuffer>(lveDevice,
            sizeof(GlobalUbo), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        uboBuffers[i]->map();
    }

    texture = std::make_unique<Texture>(
        lveDevice, executablePath + "/ZappyGui/textures/meme.png");

    VkDescriptorImageInfo imageInfo = {};
    imageInfo.sampler = texture->getSampler();
    imageInfo.imageView = texture->getImageView();
    imageInfo.imageLayout = texture->getImageLayout();

    globalSetLayout =
        ZappyDescriptorSetLayout::Builder(lveDevice)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                VK_SHADER_STAGE_ALL_GRAPHICS)
            .addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                VK_SHADER_STAGE_FRAGMENT_BIT)
            .build();

    // Create descriptor pool with adequate size
    globalPool =
        ZappyDescriptorPool::Builder(lveDevice)
            .setMaxSets(ZappySwapChain::MAX_FRAMES_IN_FLIGHT *
                5) // Adjusted to fit more sets
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                ZappySwapChain::MAX_FRAMES_IN_FLIGHT * 5)
            .addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                ZappySwapChain::MAX_FRAMES_IN_FLIGHT * 5)
            .build();

    createGameObject(executablePath + "/ZappyGui/models/Grass_Block.obj",
        executablePath + "/ZappyGui/textures/Grass_Block.png", {0.f, 0.f, 0.f},
        {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f});

    std::vector<VkDescriptorSet> globalDescriptorSets(
        ZappySwapChain::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < globalDescriptorSets.size(); i++) {
        auto bufferInfo = uboBuffers[i]->descriptorInfo();
        ZappyDescriptorWriter(*globalSetLayout, *globalPool)
            .writeBuffer(0, &bufferInfo)
            .writeImage(1, &imageInfo)
            .build(globalDescriptorSets[i]);
    }

    SimpleRenderSystem simpleRenderSystem{lveDevice,
        lveRenderer.getSwapChainRenderPass(),
        globalSetLayout->getDescriptorSetLayout(), executablePath};
    PointLightSystem pointLightSystem{lveDevice,
        lveRenderer.getSwapChainRenderPass(),
        globalSetLayout->getDescriptorSetLayout(), executablePath};
    ZappyCamera camera{};

    auto viewerObject = ZappyGameObject::createGameObject();
    viewerObject.transform.translation.z = -2.5f;
    KeyboardMovementController cameraController{};

    auto currentTime = std::chrono::high_resolution_clock::now();
    while (!lveWindow.shouldClose()) {
        // set all fd clients to read
        FD_ZERO(&readfds);

        int socket_fd = this->gameContent.get()->getClient()->getSocketFd();
        FD_SET(socket_fd, &readfds);
        FD_SET(STDIN_FILENO, &readfds);

        // Calculate the maximum file descriptor
        int max_fd = std::max(socket_fd, STDIN_FILENO) + 1;

        // Set timeout for select
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 16000; // 16 ms timeout for roughly 60 FPS

        // Wait for an activity on one of the file descriptors
        int activity = select(max_fd, &readfds, NULL, NULL, &timeout);
        if (activity < 0) {
            perror("select");
            exit(84);
        }

        // Check if there is activity on the socket file descriptor
        if (activity > 0 && FD_ISSET(socket_fd, &readfds)) {
            this->gameContent.get()->getClient().get()->receiveFromServer();
            this->gameContent.get()->setSplitedBuffer(
                this->gameContent.get()->getClient().get()->getBuffer());
            if (this->gameContent.get()->getPointerToFunction().find(
                    this->gameContent.get()->getSplitedBuffer()[0]) !=
                this->gameContent.get()->getPointerToFunction().end()) {
                try {
                    std::cout << "Command: "
                              << this->gameContent.get()->getSplitedBuffer()[0]
                              << std::endl;
                    this->gameContent.get()->getPointerToFunction()
                        [this->gameContent.get()->getSplitedBuffer()[0]]();
                } catch (const std::exception &e) {
                    std::cerr << e.what() << std::endl;
                }
            } else {
                std::cerr << "Unknown command: "
                          << this->gameContent.get()->getSplitedBuffer()[0]
                          << std::endl;
            }
            this->gameContent.get()->getClient().get()->getBuffer().clear();
            this->gameContent.get()->getSplitedBuffer().clear();
        }

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

            // order here matters
            simpleRenderSystem.renderGameObjects(frameInfo);
            pointLightSystem.render(frameInfo);

            lveRenderer.endSwapChainRenderPass(commandBuffer);
            lveRenderer.endFrame();
        }
    }

    vkDeviceWaitIdle(lveDevice.device());
}

std::string ZappyGui::getExecutablePath()
{
    char buffer[1024];
    ssize_t count = readlink("/proc/self/exe", buffer, sizeof(buffer));
    if (count == -1) {
        throw zappy::ReadLinkFailedException();
    }
    // cut the binary name from the path
    for (ssize_t i = count; i >= 0; i--) {
        if (buffer[i] == '/') {
            count = i;
            break;
        }
    }
    return std::string(buffer, (count > 0) ? count : 0);
}

void ZappyGui::loadGameObjects()
{
    // std::vector<std::vector<glm::vec3>> trantPositions = {
    //     {{-0.1f, 0.0f, 0.0f}, {-0.4f, 0.0f, 0.0f}, {-0.8f, 0.0f, 0.0f},
    //     {-1.2f, 0.0f, 0.0f}},
    //     {{0.1f, 0.0f, 0.0f}, {0.4f, 0.0f, 0.0f}, {0.8f, 0.0f, 0.0f}, {1.2f,
    //     0.0f, 0.0f}}
    // };

    std::shared_ptr<ZappyModel> lveModel = ZappyModel::createModelFromFile(lveDevice, executablePath + "/ZappyGui/models/smooth_vase.obj");

    // std::vector<std::string> teamNames = {"Team-A", "Team-B"};

    this->gameContent.get()->createTeam(lveModel, "Team-A", {0.0f, 0.0f, 0.0f});
    this->gameContent.get()->addTrantorian(lveModel, "Team-A", {8.0f, 8.0f, 8.0f});

}

std::unique_ptr<GameContent> &ZappyGui::getGameContent()
{
    return gameContent;
}

void ZappyGui::createGameObject(const std::string &modelPath,
    const std::string &texturePath, const glm::vec3 &position,
    const glm::vec3 &rotation, const glm::vec3 &scale)
{
    std::shared_ptr<ZappyModel> modelObject =
        ZappyModel::createModelFromFile(lveDevice, modelPath);
    ZappyGameObject object = ZappyGameObject::createGameObject();
    object.model = modelObject;
    object.transform.translation = position;
    object.transform.rotation = rotation;
    object.transform.scale = scale;

    if (!texturePath.empty()) {
        object.hasDescriptorSet = true;

        object.texture = std::make_unique<Texture>(
            lveDevice, texturePath);
        object.imageInfo.sampler = object.texture->getSampler();
        object.imageInfo.imageView = object.texture->getImageView();
        object.imageInfo.imageLayout = object.texture->getImageLayout();

        object.descriptorSets.resize(ZappySwapChain::MAX_FRAMES_IN_FLIGHT);

        for (int i = 0; i < uboBuffers.size(); i++) {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            ZappyDescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .writeImage(1, &object.imageInfo)
                .build(object.descriptorSets[i]);
        }
    }
    gameObjects.emplace(object.getId(), std::move(object));
}

} // namespace zappy
