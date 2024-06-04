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
#include "KeyboardMovementController.hpp"
#include "systems/PointLightSystem.hpp"
#include "systems/SimpleRenderSystem.hpp"
#include "ErrorHandling.hpp"
#include "Trantorian.hpp"
#include "GameContent.hpp"

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

FirstApp::FirstApp()
{
    this->client = std::make_unique<Client>();
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

FirstApp::~FirstApp() {}

void FirstApp::run()
{
    this->getClient()->connectToServer();
    fd_set readfds;

    std::vector<std::unique_ptr<ZappyBuffer>> uboBuffers(
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

    auto globalSetLayout =
        ZappyDescriptorSetLayout::Builder(lveDevice)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                VK_SHADER_STAGE_ALL_GRAPHICS)
            .addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                VK_SHADER_STAGE_FRAGMENT_BIT)
            .build();

    // Create descriptor pool with adequate size
    auto globalPool = ZappyDescriptorPool::Builder(lveDevice)
        .setMaxSets(ZappySwapChain::MAX_FRAMES_IN_FLIGHT * 2)  // Adjusted to fit more sets
        .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, ZappySwapChain::MAX_FRAMES_IN_FLIGHT * 2)
        .addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, ZappySwapChain::MAX_FRAMES_IN_FLIGHT * 2)
        .build();

    // Init viking_room
    std::shared_ptr<ZappyModel> grassBlockModel = ZappyModel::createModelFromFile(
        lveDevice, executablePath + "/ZappyGui/models/Grass_Block.obj");
    ZappyGameObject grassBlock = ZappyGameObject::createGameObject();
    grassBlock.model = grassBlockModel;
    grassBlock.transform.translation = {0.f, 0.f, 0.f};
    grassBlock.transform.scale = {1.f, 1.f, 1.f};
    grassBlock.hasDescriptorSet = true;

    std::unique_ptr<Texture> grassBlockTexture = std::make_unique<Texture>(
        lveDevice, executablePath + "/ZappyGui/textures/Grass_Block.png");
    VkDescriptorImageInfo grassBlockImageInfo = {};
    grassBlockImageInfo.sampler = grassBlockTexture->getSampler();
    grassBlockImageInfo.imageView = grassBlockTexture->getImageView();
    grassBlockImageInfo.imageLayout = grassBlockTexture->getImageLayout();

    // Initialize descriptor sets array for vikingRoom
    grassBlock.descriptorSets.resize(ZappySwapChain::MAX_FRAMES_IN_FLIGHT);

    for (int i = 0; i < uboBuffers.size(); i++) {
        auto bufferInfo = uboBuffers[i]->descriptorInfo();
        ZappyDescriptorWriter(*globalSetLayout, *globalPool)
            .writeBuffer(0, &bufferInfo)
            .writeImage(1, &grassBlockImageInfo)
            .build(grassBlock.descriptorSets[i]);
    }

    gameObjects.emplace(grassBlock.getId(), std::move(grassBlock));

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

        int socket_fd = this->client.get()->getSocketFd();
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
            this->client.get()->receiveFromServer();
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

std::string FirstApp::getExecutablePath()
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

void FirstApp::loadGameObjects()
{
    std::vector<std::vector<glm::vec3>> trantPositions = {
        {{-0.1f, 0.0f, 0.0f}, {-0.4f, 0.0f, 0.0f}, {-0.8f, 0.0f, 0.0f}, {-1.2f, 0.0f, 0.0f}},
        {{0.1f, 0.0f, 0.0f}, {0.4f, 0.0f, 0.0f}, {0.8f, 0.0f, 0.0f}, {1.2f, 0.0f, 0.0f}}
    };

    std::shared_ptr<ZappyModel> lveModel = ZappyModel::createModelFromFile(lveDevice, executablePath + "/ZappyGui/models/smooth_vase.obj");

    std::vector<std::string> teamNames = {"Team-A", "Team-B"};

    for (int i = 0; i < 2; i++) { // Number of teams
        gameContent.addTeam(teamNames[i]);

        for (int j = 0; j < 4; j++) { // Number of trantorians in each team
            auto object = ZappyGameObject::createGameObject();
            object.model = lveModel;
            object.transform.translation = trantPositions[i][j];
            object.transform.scale = {3.f, 1.5f, 3.f};
            gameObjects.emplace(object.getId(), std::move(object));

            // gameContent.addTrantorian(teamNames[i], object, j);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // std::shared_ptr<ZappyModel> lveModel = ZappyModel::createModelFromFile(
    //     lveDevice, executablePath + "/ZappyGui/models/flat_vase.obj");
    // auto flatVase = ZappyGameObject::createGameObject();
    // flatVase.model = lveModel;
    // flatVase.transform.translation = {-.5f, .5f, 0.f};
    // flatVase.transform.scale = {3.f, 1.5f, 3.f};
    // gameObjects.emplace(flatVase.getId(), std::move(flatVase));

    // lveModel = ZappyModel::createModelFromFile(
    //     lveDevice, executablePath + "/ZappyGui/models/smooth_vase.obj");
    // auto smoothVase = ZappyGameObject::createGameObject();
    // smoothVase.model = lveModel;
    // smoothVase.transform.translation = {.5f, .5f, 0.f};
    // smoothVase.transform.scale = {3.f, 1.5f, 3.f};
    // gameObjects.emplace(smoothVase.getId(), std::move(smoothVase));

    // lveModel = ZappyModel::createModelFromFile(
    //     lveDevice, executablePath + "/ZappyGui/models/quad.obj");
    // auto floor = ZappyGameObject::createGameObject();
    // floor.model = lveModel;
    // floor.transform.translation = {0.f, .5f, 0.f};
    // floor.transform.scale = {3.f, 1.f, 3.f};
    // gameObjects.emplace(floor.getId(), std::move(floor));

    std::vector<glm::vec3> lightColors{
        {1.f, .1f, .1f}, {.1f, .1f, 1.f}, {.1f, 1.f, .1f}, {1.f, 1.f, .1f},
        {.1f, 1.f, 1.f}, {1.f, 1.f, 1.f}
    };

    for (int i = 0; i < lightColors.size(); i++) {
        auto pointLight = ZappyGameObject::makePointLight(0.2f);
        pointLight.color = lightColors[i];
        auto rotateLight = glm::rotate(glm::mat4(1.f),
            (i * glm::two_pi<float>()) / lightColors.size(), {0.f, -1.f, 0.f});
        pointLight.transform.translation =
            glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
        gameObjects.emplace(pointLight.getId(), std::move(pointLight));
    }
}

} // namespace zappy
