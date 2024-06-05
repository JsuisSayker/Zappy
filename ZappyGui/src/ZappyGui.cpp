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
    executablePath = getExecutablePath();
    globalPool = ZappyDescriptorPool::Builder(lveDevice)
                     .setMaxSets(ZappySwapChain::MAX_FRAMES_IN_FLIGHT)
                     .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                         ZappySwapChain::MAX_FRAMES_IN_FLIGHT)
                     .addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                         ZappySwapChain::MAX_FRAMES_IN_FLIGHT)
                     .build();
    loadGameObjects();
    this->client = std::make_shared<Client>();
    this->map_ = std::make_unique<Map>();
    this->_pointerToFunction["msz"] =
        std::bind(&ZappyGui::msz, this, std::placeholders::_1);
    this->_pointerToFunction["bct"] =
        std::bind(&ZappyGui::bct, this, std::placeholders::_1);
    this->_pointerToFunction["mct"] =
        std::bind(&ZappyGui::mct, this, std::placeholders::_1);
    this->_pointerToFunction["tna"] =
        std::bind(&ZappyGui::tna, this, std::placeholders::_1);
    this->_pointerToFunction["ppo"] =
        std::bind(&ZappyGui::ppo, this, std::placeholders::_1);
    this->_pointerToFunction["plv"] =
        std::bind(&ZappyGui::plv, this, std::placeholders::_1);
    this->_pointerToFunction["pin"] =
        std::bind(&ZappyGui::pin, this, std::placeholders::_1);
    this->_pointerToFunction["sgt"] =
        std::bind(&ZappyGui::sgt, this, std::placeholders::_1);
    this->_pointerToFunction["sst"] =
        std::bind(&ZappyGui::sst, this, std::placeholders::_1);
    this->_pointerToFunction["pex"] =
        std::bind(&ZappyGui::pex, this, std::placeholders::_1);
    this->_pointerToFunction["pbc"] =
        std::bind(&ZappyGui::pbc, this, std::placeholders::_1);
    this->_pointerToFunction["pic"] =
        std::bind(&ZappyGui::pic, this, std::placeholders::_1);
    this->_pointerToFunction["pie"] =
        std::bind(&ZappyGui::pie, this, std::placeholders::_1);
    this->_pointerToFunction["pfk"] =
        std::bind(&ZappyGui::pfk, this, std::placeholders::_1);
    this->_pointerToFunction["pdr"] =
        std::bind(&ZappyGui::pdr, this, std::placeholders::_1);
    this->_pointerToFunction["pgt"] =
        std::bind(&ZappyGui::pgt, this, std::placeholders::_1);
    this->_pointerToFunction["pdi"] =
        std::bind(&ZappyGui::pdi, this, std::placeholders::_1);
    this->_pointerToFunction["enw"] =
        std::bind(&ZappyGui::enw, this, std::placeholders::_1);
    this->_pointerToFunction["eht"] =
        std::bind(&ZappyGui::eht, this, std::placeholders::_1);
    this->_pointerToFunction["ebo"] =
        std::bind(&ZappyGui::ebo, this, std::placeholders::_1);
    this->_pointerToFunction["edi"] =
        std::bind(&ZappyGui::edi, this, std::placeholders::_1);
    this->_pointerToFunction["WELCOME"] =
        std::bind(&ZappyGui::welcome, this, std::placeholders::_1);
}

ZappyGui::~ZappyGui() {}

void ZappyGui::run()
{
    this->getClient().get()->connectToServer();
    fd_set readfds;

    uboBuffers.resize(ZappySwapChain::MAX_FRAMES_IN_FLIGHT);
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
    globalPool = ZappyDescriptorPool::Builder(lveDevice)
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

        int socket_fd = this->getClient().get()->getSocketFd();
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
            this->getClient().get()->receiveFromServer();
            this->bufferToSplitedBuffer(this->getClient().get()->getBuffer());
            for (auto &actualCommand : this->getSplitedBuffer()) {
                if (this->getPointerToFunction().find(actualCommand[0]) !=
                    this->getPointerToFunction().end()) {
                    try {
                        this->getPointerToFunction()[actualCommand[0]](
                            actualCommand);
                    } catch (const std::exception &e) {
                        std::cerr << e.what() << std::endl;
                    }
                } else {
                    std::cerr << "Unknown command: " << actualCommand[0]
                              << std::endl;
                }
            }
            this->getClient().get()->getBuffer().clear();
            this->getSplitedBuffer().clear();
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

    std::shared_ptr<ZappyModel> lveModel = ZappyModel::createModelFromFile(
        lveDevice, executablePath + "/ZappyGui/models/smooth_vase.obj");

    // std::vector<std::string> teamNames = {"Team-A", "Team-B"};

    this->createTeam(lveModel, "Team-A", {0.0f, 0.0f, 0.0f});
    this->addTrantorian(lveModel, "Team-A", {8.0f, 8.0f, 8.0f});
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

    if (texturePath.size() > 0) {
        object.hasDescriptorSet = true;

        object.texture = std::make_unique<Texture>(lveDevice, texturePath);
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

void ZappyGui::setPointerToFunction(
    std::unordered_map<std::string, ZappyGui::FunctionPtr> &pointerToFunction)
{
    this->_pointerToFunction = pointerToFunction;
}

/**
 * @brief Gets the pointer to the function.
 *
 * @return The pointer to the function.
 */
std::unordered_map<std::string, ZappyGui::FunctionPtr> &
ZappyGui::getPointerToFunction()
{
    return this->_pointerToFunction;
}

void ZappyGui::msz(std::vector<std::string> actualCommand)
{
    if (actualCommand.size() != 3) {
        std::cerr << "msz: invalid number of arguments" << std::endl;
        return;
    }

    int width = std::stoi(actualCommand[1]);
    int height = std::stoi(actualCommand[2]);

    this->map_.get()->setHeight(height);
    this->map_.get()->setWidth(width);
}

void ZappyGui::bct(std::vector<std::string> actualCommand)
{
    if (actualCommand.size() != 10) {
        std::cerr << "bct: invalid number of arguments" << std::endl;
        return;
    }

    int x = std::stoi(actualCommand[1]);
    int y = std::stoi(actualCommand[2]);
    int food = std::stoi(actualCommand[3]);
    int linemate = std::stoi(actualCommand[4]);
    int deraumere = std::stoi(actualCommand[5]);
    int sibur = std::stoi(actualCommand[6]);
    int mendiane = std::stoi(actualCommand[7]);
    int phiras = std::stoi(actualCommand[8]);
    int thystame = std::stoi(actualCommand[9]);
}

void ZappyGui::mct(std::vector<std::string> actualCommand)
{
    std::cout << "mct" << std::endl;
}

void ZappyGui::tna(std::vector<std::string> actualCommand)
{
    std::cout << "tna" << std::endl;
}

void ZappyGui::ppo(std::vector<std::string> actualCommand)
{
    std::cout << "ppo" << std::endl;
}

void ZappyGui::plv(std::vector<std::string> actualCommand)
{
    std::cout << "plv" << std::endl;
}

void ZappyGui::pin(std::vector<std::string> actualCommand)
{
    std::cout << "pin" << std::endl;
}

void ZappyGui::sgt(std::vector<std::string> actualCommand)
{
    std::cout << "sgt" << std::endl;
}

void ZappyGui::sst(std::vector<std::string> actualCommand)
{
    std::cout << "sst" << std::endl;
}

void ZappyGui::pex(std::vector<std::string> actualCommand)
{
    std::cout << "pex" << std::endl;
}

void ZappyGui::pbc(std::vector<std::string> actualCommand)
{
    std::cout << "pbc" << std::endl;
}

void ZappyGui::pic(std::vector<std::string> actualCommand)
{
    std::cout << "pic" << std::endl;
}

void ZappyGui::pie(std::vector<std::string> actualCommand)
{
    std::cout << "pie" << std::endl;
}

void ZappyGui::pfk(std::vector<std::string> actualCommand)
{
    std::cout << "pfk" << std::endl;
}

void ZappyGui::pdr(std::vector<std::string> actualCommand)
{
    std::cout << "pdr" << std::endl;
}

void ZappyGui::pgt(std::vector<std::string> actualCommand)
{
    std::cout << "pgt" << std::endl;
}

void ZappyGui::pdi(std::vector<std::string> actualCommand)
{
    std::cout << "pdi" << std::endl;
}

void ZappyGui::enw(std::vector<std::string> actualCommand)
{
    std::cout << "enw" << std::endl;
}

void ZappyGui::eht(std::vector<std::string> actualCommand)
{
    std::cout << "eht" << std::endl;
}

void ZappyGui::ebo(std::vector<std::string> actualCommand)
{
    std::cout << "ebo" << std::endl;
}

void ZappyGui::edi(std::vector<std::string> actualCommand)
{
    std::cout << "edi" << std::endl;
}

void ZappyGui::welcome(std::vector<std::string> actualCommand)
{
    std::cout << "WELCOME" << std::endl;
    dprintf(this->client.get()->getSocketFd(), "GRAPHIC\n");
}

void ZappyGui::bufferToSplitedBuffer(std::string buffer)
{
    // split the bufer by \n
    std::vector<std::string> bufferSplited;
    std::string token;
    std::istringstream tokenStream(buffer);
    while (std::getline(tokenStream, token, '\n')) {
        bufferSplited.push_back(token);
    }
    // split each line by space
    for (auto &i : bufferSplited) {
        std::vector<std::string> line;
        std::string token;
        std::istringstream tokenStream(i);
        while (std::getline(tokenStream, token, ' ')) {
            line.push_back(token);
        }
        this->splitedBuffer_.push_back(line);
    }
}

std::vector<std::vector<std::string>> &ZappyGui::getSplitedBuffer()
{
    return this->splitedBuffer_;
}

void ZappyGui::createTeam(std::shared_ptr<ZappyModel> lveModel,
    const std::string &teamName, const glm::vec3 &position)
{
    auto object = std::make_shared<zappy::ZappyGameObject>(
        zappy::ZappyGameObject::createGameObject());
    object->model = lveModel;
    object->transform.translation = position;
    object->transform.scale = {3.f, 1.5f, 3.f};
    gameObjects.emplace(object->getId(), std::move(*object));

    auto pointLight = std::make_shared<zappy::ZappyGameObject>(
        zappy::ZappyGameObject::makePointLight(0.2f));
    pointLight->color = {1.0f, 1.0f,
        1.0f}; ////////////////////////////////////////////////////////////
               /// dont forget to change to generate random color
    gameObjects.emplace(pointLight->getId(), std::move(*pointLight));

    this->teamsColors_[teamName] = pointLight->color;

    // Trantorian newTrantorian(
    //     object, pointLight, teamName, 0); /////////// change number
    // trantorians_.emplace_back(newTrantorian);
}

void ZappyGui::addTrantorian(std::shared_ptr<ZappyModel> lveModel,
    const std::string &teamName, const glm::vec3 &position)
{
    auto object = std::make_shared<zappy::ZappyGameObject>(
        zappy::ZappyGameObject::createGameObject());
    object->model = lveModel;
    object->transform.translation = position;
    object->transform.scale = {3.f, 1.5f, 3.f};
    gameObjects.emplace(object->getId(), std::move(*object));

    auto pointLight = std::make_shared<zappy::ZappyGameObject>(
        zappy::ZappyGameObject::makePointLight(0.2f));
    pointLight->color = this->teamsColors_[teamName];
    gameObjects.emplace(pointLight->getId(), std::move(*pointLight));

    // Trantorian newTrantorian(
    //     object, pointLight, teamName, 0); /////////// change number
    // trantorians_.emplace_back(newTrantorian);
}
} // namespace zappy
