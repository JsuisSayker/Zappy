/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ZappyGui
*/

#include "ZappyGui.hpp"
#include <memory>

#include "Buffer.hpp"
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
    // loadGameObjects();
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
    this->_pointerToFunction["pnw"] =
        std::bind(&ZappyGui::pnw, this, std::placeholders::_1);
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
                         100) // Adjusted to fit more sets
                     .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                         ZappySwapChain::MAX_FRAMES_IN_FLIGHT * 100)
                     .addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                         ZappySwapChain::MAX_FRAMES_IN_FLIGHT * 100)
                     .build();

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

    viewerObject = ZappyGameObject::createGameObject();
    KeyboardMovementController cameraController{};

    std::thread reader(&Client::receiveFromServer, this->client.get());

    auto currentTime = std::chrono::high_resolution_clock::now();
    int socket_fd = this->getClient().get()->getSocketFd();
    while (!lveWindow.shouldClose()) {

        glfwPollEvents();
        std::unique_lock<std::mutex> lock(this->getClient().get()->_mutex);
        auto commandTime = std::chrono::high_resolution_clock::now();
        auto endTime = commandTime + std::chrono::milliseconds(16);
        while (!this->getClient().get()->getQueue().empty() &&
            commandTime < endTime) {

            std::vector<std::string> command =
                this->getClient().get()->popFromQueue();
            if (this->getPointerToFunction().find(command[0]) !=
                this->getPointerToFunction().end()) {
                try {
                    // Wrap actualCommand in a vector and call the function
                    this->getPointerToFunction()[command[0]](command);
                } catch (const std::exception &e) {
                    std::cerr << e.what() << std::endl;
                }
            } else {
                std::cerr << "Unknown command: " << command[0] << std::endl;
            }
            commandTime = std::chrono::high_resolution_clock::now();
        }
        lock.unlock();

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
                globalDescriptorSets[frameIndex], textureObjects, gameObjects};

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
    std::cout << "Closing connection" << std::endl;
    this->getClient().get()->running = false;
    reader.join();

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
    addTrantorian("Team-A", {0.f, 0.f, 0.f}, 1,
        2); //////////////////////////////////////////////////////////////////////////
    updateTrantorianPosition(1, {1.f, 0.f, 1.f}, 3);
}

ZappyGameObject::id_t ZappyGui::createGameObject(const std::string &modelPath,
    const std::string &texturePath, const glm::vec3 &position,
    const glm::vec3 &rotation, const glm::vec3 &scale, bool hasTexture)
{
    ZappyGameObject object = ZappyGameObject::createGameObject();
    for (int i = 0; i < modelObjects.size(); i++) {
        if (modelObjects[i].second == modelPath) {
            object.model = modelObjects[i].first;
        }
    }
    if (object.model == nullptr) {
        std::shared_ptr<ZappyModel> modelObject =
            ZappyModel::createModelFromFile(lveDevice, modelPath);
        object.model = modelObject;
    }
    object.transform.translation = position;
    object.transform.rotation = rotation;
    object.transform.scale = scale;

    if (hasTexture) {
        int index = 0;
        object.hasDescriptorSet = true;
        for (auto &textureObject : textureObjects) {
            if (textureObject.second.second == texturePath) {
                object.indexDescriptorSet = index;
                gameObjects.emplace(object.getId(), std::move(object));
                return object.getId();
            }
            index++;
        }

        std::shared_ptr<Texture> texture =
            std::make_shared<Texture>(lveDevice, texturePath);
        object.imageInfo.sampler = texture->getSampler();
        object.imageInfo.imageView = texture->getImageView();
        object.imageInfo.imageLayout = texture->getImageLayout();

        std::vector<VkDescriptorSet> descriptorSets(
            ZappySwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < uboBuffers.size(); i++) {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            ZappyDescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .writeImage(1, &object.imageInfo)
                .build(descriptorSets[i]);
        }
        textureObjects.push_back(std::make_pair(
            descriptorSets, std::make_pair(texture, texturePath)));
        object.indexDescriptorSet = textureObjects.size() - 1;
    }
    gameObjects.emplace(object.getId(), std::move(object));
    return object.getId();
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

    int width;
    int height;
    try {
        width = std::stoi(actualCommand[1]);
        height = std::stoi(actualCommand[2]);
    } catch (const std::exception &e) {
        return;
    }

    if (width == this->map_.get()->getWidth() &&
        height == this->map_.get()->getHeight())
        return;
    this->map_.get()->setHeight(height);
    this->map_.get()->setWidth(width);
    viewerObject.transform.translation.x = width / 2.f;
    viewerObject.transform.translation.y = -width / 1.5f;
    if (width < height) {
        viewerObject.transform.translation.y = -height / 1.5f;
    }
    viewerObject.transform.translation.z = height / 2.f;
    viewerObject.transform.rotation.x = -40.f;
    std::vector<std::vector<resources>> map;
    for (int i = 0; i < width; i++) {
        std::vector<resources> line;
        for (int j = 0; j < height; j++) {
            resources res;
            line.push_back(res);
        }
        map.push_back(line);
    }
    this->map_.get()->setMap(map);
    this->getClient().get()->map = *this->map_.get();
    this->createMap(width, height);
}

void ZappyGui::bct(std::vector<std::string> actualCommand)
{
    if (actualCommand.size() != 10) {
        std::cerr << "bct: invalid number of arguments" << std::endl;
        return;
    }

    int x;
    int y;
    int food;
    int linemate;
    int deraumere;
    int sibur;
    int mendiane;
    int phiras;
    int thystame;
    try {
        x = std::stoi(actualCommand[1]);
        y = std::stoi(actualCommand[2]);
        food = std::stoi(actualCommand[3]);
        linemate = std::stoi(actualCommand[4]);
        deraumere = std::stoi(actualCommand[5]);
        sibur = std::stoi(actualCommand[6]);
        mendiane = std::stoi(actualCommand[7]);
        phiras = std::stoi(actualCommand[8]);
        thystame = std::stoi(actualCommand[9]);
    } catch (const std::exception &e) {
        return;
    }
    std::vector<std::vector<resources>> map = this->map_.get()->getMap();
    int actualFood = map[x][y].food.size();
    int actualLinemate = map[x][y].linemate.size();
    int actualDeraumere = map[x][y].deraumere.size();
    int actualSibur = map[x][y].sibur.size();
    int actualMendiane = map[x][y].mendiane.size();
    int actualPhiras = map[x][y].phiras.size();
    int actualThystame = map[x][y].thystame.size();

    if (food >= actualFood) {
        for (int i = 0; i < food - actualFood; i++) {
            map[x][y].food.push_back(
                createGameObject(executablePath + "/ZappyGui/models/food.obj",
                    executablePath + "/ZappyGui/textures/food.png",
                    {static_cast<float>(x), -0.125f, static_cast<float>(y)},
                    {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}, true));
        }
    } else {
        for (int i = actualFood; i > food; i--) {
            removeGameObject(map[x][y].food.back());
            map[x][y].food.pop_back();
        }
    }
    if (linemate >= actualLinemate) {
        for (int i = 0; i < linemate - actualLinemate; i++) {
            map[x][y].linemate.push_back(
                createGameObject(executablePath + "/ZappyGui/models/linemate.obj",
                    executablePath + "/ZappyGui/textures/linemate.png",
                    {static_cast<float>(x) - 0.3f, -0.125f,
                        static_cast<float>(y) - 0.3f},
                    {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}, true));
        }
    } else {
        for (int i = actualLinemate; i > linemate; i--) {
            removeGameObject(map[x][y].linemate.back());
            map[x][y].linemate.pop_back();
        }
    }
    if (deraumere >= actualDeraumere) {
        for (int i = 0; i < deraumere - actualDeraumere; i++) {
            map[x][y].deraumere.push_back(
                createGameObject(executablePath + "/ZappyGui/models/deraumere.obj",
                    executablePath + "/ZappyGui/textures/deraumere.png",
                    {static_cast<float>(x) + 0.3f, -0.125f,
                        static_cast<float>(y) + 0.3f},
                    {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}, true));
        }
    } else {
        for (int i = actualDeraumere; i > deraumere; i--) {
            removeGameObject(map[x][y].deraumere.back());
            map[x][y].deraumere.pop_back();
        }
    }
    if (sibur >= actualSibur) {
        for (int i = 0; i < sibur - actualSibur; i++) {
            map[x][y].sibur.push_back(
                createGameObject(executablePath + "/ZappyGui/models/sibur.obj",
                    executablePath + "/ZappyGui/textures/sibur.png",
                    {static_cast<float>(x) - 0.3f, -0.125f,
                        static_cast<float>(y) + 0.3f},
                    {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}, true));
        }
    } else {
        for (int i = actualSibur; i > sibur; i--) {
            removeGameObject(map[x][y].sibur.back());
            map[x][y].sibur.pop_back();
        }
    }
    if (mendiane >= actualMendiane) {
        for (int i = 0; i < mendiane - actualMendiane; i++) {
            map[x][y].mendiane.push_back(
                createGameObject(executablePath + "/ZappyGui/models/mendiane.obj",
                    executablePath + "/ZappyGui/textures/mendiane.png",
                    {static_cast<float>(x) + 0.3f, -0.125f,
                        static_cast<float>(y) - 0.3f},
                    {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}, true));
        }
    } else {
        for (int i = actualMendiane; i > mendiane; i--) {
            removeGameObject(map[x][y].mendiane.back());
            map[x][y].mendiane.pop_back();
        }
    }
    if (phiras >= actualPhiras) {
        for (int i = 0; i < phiras - actualPhiras; i++) {
            map[x][y].phiras.push_back(
                createGameObject(executablePath + "/ZappyGui/models/phiras.obj",
                    executablePath + "/ZappyGui/textures/phiras.png",
                    {static_cast<float>(x) - 0.3f, -0.125f,
                        static_cast<float>(y)},
                    {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}, true));
        }
    } else {
        for (int i = actualPhiras; i > phiras; i--) {
            removeGameObject(map[x][y].phiras.back());
            map[x][y].phiras.pop_back();
        }
    }
    if (thystame >= actualThystame) {
        for (int i = 0; i < thystame - actualThystame; i++) {
            map[x][y].thystame.push_back(
                createGameObject(executablePath + "/ZappyGui/models/thystame.obj",
                    executablePath + "/ZappyGui/textures/thystame.png",
                    {static_cast<float>(x) + 0.3f, -0.125f,
                        static_cast<float>(y)},
                    {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}, true));
        }
    } else {
        for (int i = actualThystame; i > thystame; i--) {
            removeGameObject(map[x][y].thystame.back());
            map[x][y].thystame.pop_back();
        }
    }
    this->map_.get()->setMap(map);
    this->getClient().get()->map = *this->map_.get();
}

void ZappyGui::mct(std::vector<std::string> actualCommand)
{
    std::cout << "mct" << std::endl;
}

void ZappyGui::tna(std::vector<std::string> actualCommand)
{
    if (actualCommand.size() != 2) {
        std::cerr << "tna: invalid number of arguments" << std::endl;
        return;
    }

    for (auto &i : this->teamsColors_) {
        if (i.first == actualCommand[1]) {
            std::cerr << "tna: team already exists" << std::endl;
            return;
        }
    }
    std::string teamName = actualCommand[1];
    this->teamsColors_.emplace(teamName,
        glm::vec3(static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
            static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
            static_cast<float>(rand()) / static_cast<float>(RAND_MAX)));
}

void ZappyGui::pnw(std::vector<std::string> actualCommand)
{
    if (actualCommand.size() != 7) {
        std::cerr << "pnw: invalid number of arguments" << std::endl;
        return;
    }
    int trantorianId;
    int x;
    int y;
    int orientation;
    int level;
    std::string teamName;
    try {
        trantorianId = std::stoi(actualCommand[1]);
        x = std::stoi(actualCommand[2]);
        y = std::stoi(actualCommand[3]);
        orientation = std::stoi(actualCommand[4]);
        level = std::stoi(actualCommand[5]);
        teamName = actualCommand[6];
    } catch (const std::exception &e) {
        return;
    }

    this->addTrantorian(teamName,
        {static_cast<float>(x), 0.0f, static_cast<float>(y)}, trantorianId,
        orientation);
}

void ZappyGui::ppo(std::vector<std::string> actualCommand)
{
    if (actualCommand.size() != 5) {
        std::cerr << "ppo: invalid number of arguments" << std::endl;
        return;
    }
    int playerNumber;
    int x;
    int y;
    int orientation;
    try {
        playerNumber = std::stoi(actualCommand[1]);
        x = std::stoi(actualCommand[2]);
        y = std::stoi(actualCommand[3]);
        orientation = std::stoi(actualCommand[4]);
    } catch (const std::exception &e) {
        return;
    }
    this->updateTrantorianPosition(playerNumber,
        {static_cast<float>(x), 0.0f, static_cast<float>(y)}, orientation);
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
    if (actualCommand.size() != 2) {
        std::cerr << "sgt: invalid number of arguments" << std::endl;
        return;
    }

    int timeUnit = std::stoi(actualCommand[1]);

    this->_timeUnit = timeUnit;
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

void ZappyGui::enw(std::vector<std::string> actualCommand) {}

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

void ZappyGui::addTrantorian(const std::string &teamName,
    const glm::vec3 &position, int playerNumber, int orientation)
{
    glm::vec3 rotation;

    if (orientation == 1)
        rotation = {0.f, 0.f, 0.f};
    else if (orientation == 2)
        rotation = {0.f, 1.55f, 0.f};
    else if (orientation == 3)
        rotation = {0.f, 3.14f, 0.f};
    else if (orientation == 4)
        rotation = {0.f, -1.57f, 0.f};

    ZappyGameObject::id_t ObjectId =
        createGameObject(executablePath + "/ZappyGui/models/smooth_vase.obj",
            executablePath + "/ZappyGui/textures/Steve.png", position,
            rotation, {2.f, 2.f, 2.f}, false);

    std::shared_ptr<ZappyGameObject> pointLight =
        std::make_shared<ZappyGameObject>(
            zappy::ZappyGameObject::makePointLight(0.2f));
    pointLight->color = this->teamsColors_[teamName];
    pointLight.get()->transform.translation = position;
    pointLight.get()->transform.translation.y -= 1.0f;
    gameObjects.emplace(pointLight->getId(), std::move(*pointLight));

    Trantorian newTrantorian(
        ObjectId, pointLight->getId(), teamName, playerNumber);

    this->trantorians_.emplace_back(newTrantorian);
}

void ZappyGui::updateTrantorianPosition(
    int playerNumber, const glm::vec3 &position, int orientation)
{
    for (Trantorian &trantorian : trantorians_) {
        if (trantorian.playerNumber == playerNumber) {
            for (auto &object : gameObjects) {
                if (object.first == trantorian.pointLightObject) {
                    object.second.transform.translation = position;
                    object.second.transform.translation.y -= 1.0f;
                }
                if (object.first == trantorian.trantorianObject) {
                    object.second.transform.translation = position;
                    if (orientation == 1)
                        object.second.transform.rotation = {0.f, 0.f, 0.f};
                    else if (orientation == 2)
                        object.second.transform.rotation = {0.f, 1.55f, 0.f};
                    else if (orientation == 3)
                        object.second.transform.rotation = {0.f, 3.14f, 0.f};
                    else if (orientation == 4)
                        object.second.transform.rotation = {0.f, -1.57f, 0.f};
                }
            }
        }
    }
}

void ZappyGui::createMap(int width, int height)
{
    for (float i = 0.f; i < width; i++) {
        for (float j = 0.f; j < height; j++) {
            createGameObject(
                executablePath + "/ZappyGui/models/Grass_Block.obj",
                executablePath + "/ZappyGui/textures/Grass_Block.png",
                {i, 0.f, j}, {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}, true);
        }
    }
}

void ZappyGui::removeGameObject(ZappyGameObject::id_t gameObjectId)
{
    vkDeviceWaitIdle(lveDevice.device());
    gameObjects.erase(gameObjectId);
}

} // namespace zappy
