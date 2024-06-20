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
#include <glm/gtc/type_ptr.hpp>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"
#include "imgui.h"

// std
#include <array>
#include <cassert>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <random>
#include <stdexcept>
#include <unistd.h>
namespace zappy {

void ZappyGui::initHud()
{
    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImGui_ImplGlfw_InitForVulkan(this->lveWindow.getGLFWwindow(), true);

    // Initialize ImGui for Vulkan
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = this->lveDevice.getInstance();
    init_info.PhysicalDevice = this->lveDevice.getPhysicalDevice();
    init_info.Device = this->lveDevice.device();
    init_info.QueueFamily =
        this->lveDevice.findQueueFamilies(this->lveDevice.getPhysicalDevice())
            .graphicsFamily;
    init_info.Queue = this->lveDevice.graphicsQueue();
    init_info.PipelineCache = VK_NULL_HANDLE;
    init_info.DescriptorPool = this->globalPool.get()->descriptorPool;
    init_info.Subpass = 0;
    init_info.MinImageCount = ZappySwapChain::MAX_FRAMES_IN_FLIGHT + 1;
    init_info.ImageCount = ZappySwapChain::MAX_FRAMES_IN_FLIGHT + 1;
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.Allocator = nullptr;
    init_info.CheckVkResultFn = nullptr;
    init_info.RenderPass = this->lveRenderer.getSwapChainRenderPass();

    ImGui_ImplVulkan_Init(&init_info);

    // Upload Fonts
    VkCommandBuffer command_buffer = this->lveDevice.beginSingleTimeCommands();
    ImGui_ImplVulkan_CreateFontsTexture();
    this->lveDevice.endSingleTimeCommands(command_buffer);
}

void ZappyGui::drawHud()
{
    // Start ImGui frame
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Render ImGui fps in top left corner
    ImGui::SetNextWindowPos(ImVec2(10, 10));
    ImGui::SetNextWindowSize(ImVec2(0, 0));
    ImGui::Begin(
        "FPS", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::End();

    // Render ImGui time unit in top right corner
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 150, 10));
    ImGui::SetNextWindowSize(ImVec2(0, 0));
    ImGui::Begin("Time Unit", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
    ImGui::Text("Time Unit: %d", this->_timeUnit);
    ImGui::End();

    // Render ImGui wrap menu with teams colors in left side
    ImGui::SetNextWindowPos(ImVec2(10, 100), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(250, 500), ImGuiCond_FirstUseEver);
    ImGui::Begin("Teams", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
    static std::string current_team = "";

    if (ImGui::BeginCombo("Teams", current_team.c_str())) {
        for (auto &team : this->teamsColors_) {
            bool is_selected = (current_team == team.first);
            if (ImGui::Selectable(team.first.c_str(), is_selected)) {
                current_team = team.first;
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    // Display a color selector for the selected team
    if (!current_team.empty()) {
        ImGui::Separator();

        glm::vec3 glmColor1 = this->teamsColors_[current_team] / 255.0f;
        float *color = glm::value_ptr(glmColor1);

        if (ImGui::ColorEdit3("Color", color)) {
            glmColor1 = glm::vec3(color[0], color[1], color[2]);
            this->teamsColors_[current_team] = glmColor1 * 255.0f;
        }
    }

    ImGui::Separator();

    // Display trantorians for the selected team
    if (!current_team.empty()) {
        for (auto &trantorian : this->trantorians_) {
            ImGui::Button(std::to_string(trantorian.playerNumber).c_str());
            if (ImGui::IsItemClicked(0)) {
                this->showChildWindow = !this->showChildWindow;
                this->selectedPlayerNbr = trantorian.playerNumber;
            }
        }
    }
    ImGui::End();

    if (this->showChildWindow) {
        ImGui::SetNextWindowPos(ImVec2(110, 100), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(200, 180), ImGuiCond_FirstUseEver);

        ImGui::Begin(
            std::to_string(this->selectedPlayerNbr).c_str(), nullptr, 0);
        // Display trantorian informations
        for (auto &trantorian : this->trantorians_) {
            if (trantorian.playerNumber == this->selectedPlayerNbr) {
                if (trantorian.playerNumber == this->selectedPlayerNbr) {
                    if (ImGui::BeginTabBar("Player Info")) {
                        if (ImGui::BeginTabItem("General")) {
                            ImGui::Text(
                                "Player number: %d", trantorian.playerNumber);
                            ImGui::Text("Level: %d", trantorian.level);
                            ImGui::Text("Team: %s", trantorian.team.c_str());
                            ImGui::EndTabItem();
                        }
                        if (ImGui::BeginTabItem("Inventory")) {
                            ImGui::Text("Food: %d", trantorian.inventory.food);
                            ImGui::Text(
                                "Linemate: %d", trantorian.inventory.linemate);
                            ImGui::Text("Deraumere: %d",
                                trantorian.inventory.deraumere);
                            ImGui::Text(
                                "Sibur: %d", trantorian.inventory.sibur);
                            ImGui::Text(
                                "Mendiane: %d", trantorian.inventory.mendiane);
                            ImGui::Text(
                                "Phiras: %d", trantorian.inventory.phiras);
                            ImGui::Text(
                                "Thystame: %d", trantorian.inventory.thystame);
                            ImGui::EndTabItem();
                        }
                        ImGui::EndTabBar();
                    }
                    ImGui::Separator();
                }
            }
        }
        ImGui::End();
    }

    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 300,
        ImGui::GetIO().DisplaySize.y - 200));
    ImGui::SetNextWindowSize(ImVec2(300, 200));
    ImGui::Begin("Chat", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
    ImGui::Text("Chat");
    ImGui::Separator();
    for (auto &message : this->chatMessages) {
        ImGui::Text("%s : %s", message.first.c_str(), message.second.c_str());
    }
    ImGui::End();
    // Rendering ImGui
    ImGui::Render();
}

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
    this->client = std::make_shared<Client>();
    this->map_ = std::make_unique<Map>();
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
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
    this->_pointerToFunction["ebo"] =
        std::bind(&ZappyGui::ebo, this, std::placeholders::_1);
    this->_pointerToFunction["edi"] =
        std::bind(&ZappyGui::edi, this, std::placeholders::_1);
    this->_pointerToFunction["WELCOME"] =
        std::bind(&ZappyGui::welcome, this, std::placeholders::_1);
    timerManager_.startTimer("portalAnimation");
    timerManager_.startTimer("resourcesAnimation");
    resources_.emplace("food", 0);
    resources_.emplace("linemate", 0);
    resources_.emplace("deraumere", 0);
    resources_.emplace("sibur", 0);
    resources_.emplace("mendiane", 0);
    resources_.emplace("phiras", 0);
    resources_.emplace("thystame", 0);
}

void ZappyGui::processCommand()
{
    std::unique_lock<std::mutex> lock(this->getClient().get()->_mutex);
    auto commandTime = std::chrono::high_resolution_clock::now();
    auto endTime = commandTime + std::chrono::milliseconds(16);

    while (!this->getClient().get()->getQueue().empty() && commandTime < endTime) {
        std::vector<std::string> command = this->getClient().get()->popFromQueue();

        if (this->getPointerToFunction().find(command[0]) !=
            this->getPointerToFunction().end()) {
            try {
                this->getPointerToFunction()[command[0]](command);
            } catch (const std::exception &e) {
                std::cerr << "Exception: " << e.what() << std::endl;
            }
        } else {
            std::cerr << "Unknown command: " << command[0] << std::endl;
        }

        commandTime = std::chrono::high_resolution_clock::now();
    }
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

    globalPool = ZappyDescriptorPool::Builder(lveDevice)
                     .setMaxSets(ZappySwapChain::MAX_FRAMES_IN_FLIGHT * 500)
                     .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                         ZappySwapChain::MAX_FRAMES_IN_FLIGHT * 500)
                     .addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                         ZappySwapChain::MAX_FRAMES_IN_FLIGHT * 500)
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
    this->initHud();

    while (!lveWindow.shouldClose()) {
        glfwPollEvents();

        updateGame();

        processCommand();

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

            this->drawHud();
            ImGui_ImplVulkan_RenderDrawData(
                ImGui::GetDrawData(), commandBuffer);

            lveRenderer.endSwapChainRenderPass(commandBuffer);
            lveRenderer.endFrame();
        }
    }

    // Cleanup
    vkDeviceWaitIdle(lveDevice.device());
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    close(socket_fd);

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
        for (std::pair<std::vector<VkDescriptorSet>,
                 std::pair<std::shared_ptr<zappy::Texture>, std::string>>
                 &textureObject : textureObjects) {
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
            ZappyGameObject::id_t foodId;
            if (!resources_["food"].empty()) {
                foodId = createGameObject(
                    executablePath + "/ZappyGui/models/diamond_apple.obj",
                    executablePath + "/ZappyGui/textures/diamond_apple.png",
                    {static_cast<float>(x),
                        gameObjects[resources_["food"].back()]
                            .transform.translation.y,
                        static_cast<float>(y)},
                    {degreeToRadiant(180), 0.f, 0.f}, {0.2f, 0.2f, 0.2f},
                    true);
            } else {
                foodId = createGameObject(
                    executablePath + "/ZappyGui/models/diamond_apple.obj",
                    executablePath + "/ZappyGui/textures/diamond_apple.png",
                    {static_cast<float>(x), 0.f, static_cast<float>(y)},
                    {degreeToRadiant(180), 0.f, 0.f}, {0.2f, 0.2f, 0.2f},
                    true);
            }
            map[x][y].food.push_back(foodId);
            resources_["food"].push_back(foodId);
        }
    } else {
        for (int i = actualFood; i > food; i--) {
            removeGameObject(map[x][y].food.back());
            for (int j = 0; j < resources_["food"].size(); j++) {
                if (resources_["food"][j] == map[x][y].food.back()) {
                    resources_["food"].erase(resources_["food"].begin() + j);
                    break;
                }
            }
            map[x][y].food.pop_back();
        }
    }
    if (linemate >= actualLinemate) {
        for (int i = 0; i < linemate - actualLinemate; i++) {
            ZappyGameObject::id_t linemateId;
            if (!resources_["linemate"].empty()) {
                linemateId = createGameObject(
                    executablePath + "/ZappyGui/models/ingot.obj",
                    executablePath + "/ZappyGui/textures/iron_ingot.png",
                    {static_cast<float>(x) - 0.3f,
                        gameObjects[resources_["linemate"].back()]
                            .transform.translation.y,
                        static_cast<float>(y) - 0.3f},
                    {degreeToRadiant(180), 0.f, 0.f}, {0.2f, 0.2f, 0.2f},
                    true);
            } else {
                linemateId = createGameObject(
                    executablePath + "/ZappyGui/models/ingot.obj",
                    executablePath + "/ZappyGui/textures/iron_ingot.png",
                    {static_cast<float>(x) - 0.3f, 0.f,
                        static_cast<float>(y) - 0.3f},
                    {degreeToRadiant(180), 0.f, 0.f}, {0.2f, 0.2f, 0.2f},
                    true);
            }
            map[x][y].linemate.push_back(linemateId);
            resources_["linemate"].push_back(linemateId);
        }
    } else {
        for (int i = actualLinemate; i > linemate; i--) {
            removeGameObject(map[x][y].linemate.back());
            for (int j = 0; j < resources_["linemate"].size(); j++) {
                if (resources_["linemate"][j] == map[x][y].linemate.back()) {
                    resources_["linemate"].erase(
                        resources_["linemate"].begin() + j);
                    break;
                }
            }
            map[x][y].linemate.pop_back();
        }
    }
    if (deraumere >= actualDeraumere) {
        for (int i = 0; i < deraumere - actualDeraumere; i++) {
            ZappyGameObject::id_t deraumereId;
            if (!resources_["deraumere"].empty()) {
                deraumereId = createGameObject(
                    executablePath + "/ZappyGui/models/ingot.obj",
                    executablePath + "/ZappyGui/textures/gold_ingot.png",
                    {static_cast<float>(x) + 0.3f,
                        gameObjects[resources_["deraumere"].back()]
                            .transform.translation.y,
                        static_cast<float>(y) + 0.3f},
                    {degreeToRadiant(180), 0.f, 0.f}, {0.2f, 0.2f, 0.2f},
                    true);
            } else {
                deraumereId = createGameObject(
                    executablePath + "/ZappyGui/models/ingot.obj",
                    executablePath + "/ZappyGui/textures/gold_ingot.png",
                    {static_cast<float>(x) + 0.3f, 0.f,
                        static_cast<float>(y) + 0.3f},
                    {degreeToRadiant(180), 0.f, 0.f}, {0.2f, 0.2f, 0.2f},
                    true);
            }
            map[x][y].deraumere.push_back(deraumereId);
            resources_["deraumere"].push_back(deraumereId);
        }
    } else {
        for (int i = actualDeraumere; i > deraumere; i--) {
            removeGameObject(map[x][y].deraumere.back());
            for (int j = 0; j < resources_["deraumere"].size(); j++) {
                if (resources_["deraumere"][j] == map[x][y].deraumere.back()) {
                    resources_["deraumere"].erase(
                        resources_["deraumere"].begin() + j);
                    break;
                }
            }
            map[x][y].deraumere.pop_back();
        }
    }
    if (sibur >= actualSibur) {
        for (int i = 0; i < sibur - actualSibur; i++) {
            ZappyGameObject::id_t siburId;
            if (!resources_["sibur"].empty()) {
                siburId = createGameObject(
                    executablePath + "/ZappyGui/models/diamond.obj",
                    executablePath + "/ZappyGui/textures/diamond.png",
                    {static_cast<float>(x) - 0.3f,
                        gameObjects[resources_["sibur"].back()]
                            .transform.translation.y,
                        static_cast<float>(y) + 0.3f},
                    {degreeToRadiant(180), 0.f, 0.f}, {0.2f, 0.2f, 0.2f},
                    true);
            } else {
                siburId = createGameObject(
                    executablePath + "/ZappyGui/models/diamond.obj",
                    executablePath + "/ZappyGui/textures/diamond.png",
                    {static_cast<float>(x) - 0.3f, 0.f,
                        static_cast<float>(y) + 0.3f},
                    {degreeToRadiant(180), 0.f, 0.f}, {0.2f, 0.2f, 0.2f},
                    true);
            }
            map[x][y].sibur.push_back(siburId);
            resources_["sibur"].push_back(siburId);
        }
    } else {
        for (int i = actualSibur; i > sibur; i--) {
            removeGameObject(map[x][y].sibur.back());
            for (int j = 0; j < resources_["sibur"].size(); j++) {
                if (resources_["sibur"][j] == map[x][y].sibur.back()) {
                    resources_["sibur"].erase(resources_["sibur"].begin() + j);
                    break;
                }
            }
            map[x][y].sibur.pop_back();
        }
    }
    if (mendiane >= actualMendiane) {
        for (int i = 0; i < mendiane - actualMendiane; i++) {
            ZappyGameObject::id_t mendianeId;
            if (!resources_["mendiane"].empty()) {
                mendianeId = createGameObject(
                    executablePath + "/ZappyGui/models/diamond.obj",
                    executablePath + "/ZappyGui/textures/diamond_purple.png",
                    {static_cast<float>(x) + 0.3f,
                        gameObjects[resources_["mendiane"].back()]
                            .transform.translation.y,
                        static_cast<float>(y) - 0.3f},
                    {degreeToRadiant(180), 0.f, 0.f}, {0.2f, 0.2f, 0.2f},
                    true);
            } else {
                mendianeId = createGameObject(
                    executablePath + "/ZappyGui/models/diamond.obj",
                    executablePath + "/ZappyGui/textures/diamond_purple.png",
                    {static_cast<float>(x) + 0.3f, 0.f,
                        static_cast<float>(y) - 0.3f},
                    {degreeToRadiant(180), 0.f, 0.f}, {0.2f, 0.2f, 0.2f},
                    true);
            }
            map[x][y].mendiane.push_back(mendianeId);
            resources_["mendiane"].push_back(mendianeId);
        }
    } else {
        for (int i = actualMendiane; i > mendiane; i--) {
            removeGameObject(map[x][y].mendiane.back());
            for (int j = 0; j < resources_["mendiane"].size(); j++) {
                if (resources_["mendiane"][j] == map[x][y].mendiane.back()) {
                    resources_["mendiane"].erase(
                        resources_["mendiane"].begin() + j);
                    break;
                }
            }
            map[x][y].mendiane.pop_back();
        }
    }
    if (phiras >= actualPhiras) {
        for (int i = 0; i < phiras - actualPhiras; i++) {
            ZappyGameObject::id_t phirasId;
            if (!resources_["phiras"].empty()) {
                phirasId = createGameObject(
                    executablePath + "/ZappyGui/models/ingot.obj",
                    executablePath + "/ZappyGui/textures/green_ingot.png",
                    {static_cast<float>(x) - 0.3f,
                        gameObjects[resources_["phiras"].back()]
                            .transform.translation.y,
                        static_cast<float>(y)},
                    {degreeToRadiant(180), 0.f, 0.f}, {0.2f, 0.2f, 0.2f},
                    true);
            } else {
                phirasId = createGameObject(
                    executablePath + "/ZappyGui/models/ingot.obj",
                    executablePath + "/ZappyGui/textures/green_ingot.png",
                    {static_cast<float>(x) - 0.3f, 0.f, static_cast<float>(y)},
                    {degreeToRadiant(180), 0.f, 0.f}, {0.2f, 0.2f, 0.2f},
                    true);
            }
            map[x][y].phiras.push_back(phirasId);
            resources_["phiras"].push_back(phirasId);
        }
    } else {
        for (int i = actualPhiras; i > phiras; i--) {
            removeGameObject(map[x][y].phiras.back());
            for (int j = 0; j < resources_["phiras"].size(); j++) {
                if (resources_["phiras"][j] == map[x][y].phiras.back()) {
                    resources_["phiras"].erase(
                        resources_["phiras"].begin() + j);
                    break;
                }
            }
            map[x][y].phiras.pop_back();
        }
    }
    if (thystame >= actualThystame) {
        for (int i = 0; i < thystame - actualThystame; i++) {
            ZappyGameObject::id_t thystameId;
            if (!resources_["thystame"].empty()) {
                thystameId = createGameObject(
                    executablePath + "/ZappyGui/models/diamond.obj",
                    executablePath + "/ZappyGui/textures/diamond_red.png",
                    {static_cast<float>(x) + 0.3f,
                        gameObjects[resources_["thystame"].back()]
                            .transform.translation.y,
                        static_cast<float>(y)},
                    {degreeToRadiant(180), 0.f, 0.f}, {0.2f, 0.2f, 0.2f},
                    true);
            } else {
                thystameId = createGameObject(
                    executablePath + "/ZappyGui/models/diamond.obj",
                    executablePath + "/ZappyGui/textures/diamond_red.png",
                    {static_cast<float>(x) + 0.3f, 0.f, static_cast<float>(y)},
                    {degreeToRadiant(180), 0.f, 0.f}, {0.2f, 0.2f, 0.2f},
                    true);
            }
            map[x][y].thystame.push_back(thystameId);
            resources_["thystame"].push_back(thystameId);
        }
    } else {
        for (int i = actualThystame; i > thystame; i--) {
            removeGameObject(map[x][y].thystame.back());
            for (int j = 0; j < resources_["thystame"].size(); j++) {
                if (resources_["thystame"][j] == map[x][y].thystame.back()) {
                    resources_["thystame"].erase(
                        resources_["thystame"].begin() + j);
                    break;
                }
            }
            map[x][y].thystame.pop_back();
        }
    }
    this->map_.get()->setMap(map);
    this->getClient().get()->map = *this->map_.get();
}

void ZappyGui::mct(std::vector<std::string> actualCommand)
{
    // std::cout << "mct" << std::endl;
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
        glm::vec3(getRandomFloat(0.01, 1.0), getRandomFloat(0.01, 1.0),
            getRandomFloat(0.01, 1.0)));
}

void ZappyGui::pnw(std::vector<std::string> actualCommand)
{
    if (actualCommand.size() != 7) {
        std::cerr << "pnw: invalid number of arguments" << std::endl;
        return;
    }
    int playerNumber;
    int x;
    int y;
    std::string orientation;
    int level;
    std::string teamName;
    try {
        actualCommand[1].erase(actualCommand[1].begin());
        playerNumber = std::stoi(actualCommand[1]);
        x = std::stoi(actualCommand[2]);
        y = std::stoi(actualCommand[3]);
        orientation = actualCommand[4];
        level = std::stoi(actualCommand[5]);
        teamName = actualCommand[6];
    } catch (const std::exception &e) {
        return;
    }

    this->addTrantorian(teamName,
        {static_cast<float>(x), -.25f, static_cast<float>(y)}, playerNumber,
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
    std::string orientation;
    try {
        actualCommand[1].erase(actualCommand[1].begin());
        playerNumber = std::stoi(actualCommand[1]);
        x = std::stoi(actualCommand[2]);
        y = std::stoi(actualCommand[3]);
        orientation = actualCommand[4];
    } catch (const std::exception &e) {
        return;
    }
    this->updateTrantorianPosition(playerNumber,
        {static_cast<float>(x), 0.f, static_cast<float>(y)}, orientation);
}

void ZappyGui::plv(std::vector<std::string> actualCommand)
{
    if (actualCommand.size() != 3) {
        std::cerr << "plv: invalid number of arguments" << std::endl;
        return;
    }

    int playerNumber;
    int level;
    try {
        actualCommand[1].erase(actualCommand[1].begin());
        playerNumber = std::stoi(actualCommand[1]);
        level = std::stoi(actualCommand[2]);
    } catch (const std::exception &e) {
        return;
    }
    for (Trantorian &Trantorian : trantorians_) {
        if (Trantorian.playerNumber == playerNumber) {
            Trantorian.level = level;
            for (auto &object : gameObjects) {
                if (object.first == Trantorian.trantorianObject)
                    object.second.transform.scale = {0.25f + 0.1f * level,
                        0.25f + 0.1f * level, 0.25f + 0.1f * level};
                object.second.transform.translation.y =
                    -0.25f - (0.1f * level);
            }
        }
    }
}

void ZappyGui::pin(std::vector<std::string> actualCommand)
{
    // std::cout << "pin" << std::endl;
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
    // std::cout << "  sst" << std::endl;
}

void ZappyGui::pex(std::vector<std::string> actualCommand)
{
    // std::cout << "pex" << std::endl;
}

void ZappyGui::pbc(std::vector<std::string> actualCommand)
{
    if (actualCommand.size() != 3) {
        std::cerr << "pbc: invalid number of arguments" << std::endl;
        return;
    }
    actualCommand[1].erase(actualCommand[1].begin());
    int playerNumber = std::stoi(actualCommand[1]);
    std::string message = actualCommand[2];

    for (Trantorian &Trantorian : trantorians_) {
        if (Trantorian.playerNumber == playerNumber) {
            chatMessages.push_back(std::make_pair(actualCommand[1], message));
        }
    }
}

void ZappyGui::pic(std::vector<std::string> actualCommand)
{
    if (actualCommand.size() < 4) {
        std::cerr << "pic: invalid number of arguments" << std::endl;
        return;
    }
    int x;
    int y;
    std::vector<int> playerNumbers;
    try {
        x = std::stoi(actualCommand[1]);
        y = std::stoi(actualCommand[2]);
        for (int i = 3; i < actualCommand.size(); i++) {
            actualCommand[i].erase(actualCommand[i].begin());
            playerNumbers.push_back(std::stoi(actualCommand[i]));
        }
    } catch (const std::exception &e) {
        return;
    }
    for (Trantorian &Trantorian : trantorians_) {
        if (Trantorian.playerNumber = playerNumbers[0])
            Trantorian.incatationInProgess = true;
    }
}

void ZappyGui::pie(std::vector<std::string> actualCommand)
{
    if (actualCommand.size() != 4) {
        std::cerr << "pie: invalid number of arguments" << std::endl;
        return;
    }
    int x;
    int y;
    int result;
    try {
        x = std::stoi(actualCommand[1]);
        y = std::stoi(actualCommand[2]);
        result = std::stoi(actualCommand[3]);
    } catch (const std::exception &e) {
        return;
    }
    for (auto &object : gameObjects) {
        if (object.second.transform.translation.x == x &&
            object.second.transform.translation.z == y) {
            for (Trantorian &Trantorian : trantorians_) {
                if (Trantorian.trantorianObject == object.first)
                    Trantorian.incatationInProgess = false;
            }
        }
    }
}

void ZappyGui::pfk(std::vector<std::string> actualCommand)
{
    if (actualCommand.size() != 2) {
        std::cerr << "pfk: invalid number of arguments" << std::endl;
        return;
    }
    actualCommand[1].erase(actualCommand[1].begin());
    int playerNumber = std::stoi(actualCommand[1]);

    this->eggLayingPose(playerNumber);
}

void ZappyGui::pdr(std::vector<std::string> actualCommand)
{
    // std::cout << "pdr" << std::endl;
}

void ZappyGui::pgt(std::vector<std::string> actualCommand)
{
    // std::cout << "pgt" << std::endl;
}

void ZappyGui::pdi(std::vector<std::string> actualCommand)
{
    if (actualCommand.size() != 2) {
        std::cerr << "pdi: invalid number of arguments" << std::endl;
        return;
    }
    actualCommand[1].erase(actualCommand[1].begin());
    int playerNumber = std::stoi(actualCommand[1]);

    this->removeTrantorian(playerNumber);
}

void ZappyGui::enw(std::vector<std::string> actualCommand)
{
    if (actualCommand.size() != 5) {
        std::cerr << "enw: invalid number of arguments" << std::endl;
        return;
    }
    actualCommand[1].erase(actualCommand[1].begin());
    actualCommand[2].erase(actualCommand[2].begin());
    int eggNumber = std::stoi(actualCommand[1]);
    int playerNumber = std::stoi(actualCommand[2]);
    int x = std::stoi(actualCommand[2]);
    int y = std::stoi(actualCommand[3]);

    this->addEgg(eggNumber, playerNumber,
        {static_cast<float>(x), -0.15f, static_cast<float>(y)});
}

void ZappyGui::ebo(std::vector<std::string> actualCommand)
{
    if (actualCommand.size() != 2) {
        std::cerr << "ebo: invalid number of arguments" << std::endl;
        return;
    }
    actualCommand[1].erase(actualCommand[1].begin());
    int eggNumber = std::stoi(actualCommand[1]);
    for (auto i = eggs_.begin(); i != eggs_.end(); i++) {
        if (i->eggNumber == eggNumber) {
            for (auto j = trantorians_.begin(); j != trantorians_.end(); j++) {
                if (j->playerNumber == i->playerNumber) {
                    this->addTrantorian(
                        j->team, i->position, trantorians_.size() + 1, 0);
                    break;
                }
            }
            removeGameObject(i->eggObjectId);
            eggs_.erase(i);
            return;
        }
    }
}

void ZappyGui::edi(std::vector<std::string> actualCommand)
{
    if (actualCommand.size() != 2) {
        std::cerr << "edi: invalid number of arguments" << std::endl;
        return;
    }
    actualCommand[1].erase(actualCommand[1].begin());
    int eggNumber = std::stoi(actualCommand[1]);
    for (auto i = eggs_.begin(); i != eggs_.end(); i++) {
        if (i->eggNumber == eggNumber) {
            removeGameObject(i->eggObjectId);
            eggs_.erase(i);
            return;
        }
    }
}

void ZappyGui::welcome(std::vector<std::string> actualCommand)
{
    std::cout << "WELCOME" << std::endl;
    dprintf(this->client.get()->getSocketFd(), "GRAPHIC\n");
}

void ZappyGui::addTrantorian(const std::string &teamName, const glm::vec3 &position, int playerNumber, std::string orientation)
{
    glm::vec3 rotation;

    if (orientation == "N")
        rotation = {0.f, 0.f, 0.f};
    else if (orientation == "E")
        rotation = {0.f, 1.55f, 0.f};
    else if (orientation == "S")
        rotation = {0.f, 3.14f, 0.f};
    else if (orientation == "W")
        rotation = {0.f, -1.57f, 0.f};

    ZappyGameObject::id_t ObjectId =
        createGameObject(executablePath + "/ZappyGui/models/Slime.obj",
            executablePath + "/ZappyGui/textures/Slime.png", position,
            rotation, {0.25f, 0.25f, 0.25f}, true);

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

void ZappyGui::removeTrantorian(int playerNumber)
{
    for (auto i = trantorians_.begin(); i != trantorians_.end(); i++) {
        if (i->playerNumber == playerNumber) {
            removeGameObject(i->pointLightObject);
            removeGameObject(i->trantorianObject);
            trantorians_.erase(i);
            return;
        }
    }
}

void ZappyGui::updateTrantorianPosition(int playerNumber, const glm::vec3 &position, std::string orientation)
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
                    object.second.transform.translation.y =
                        object.second.transform.scale.y * -1;
                    if (orientation == "N")
                        object.second.transform.rotation = {0.f, 0.f, 0.f};
                    else if (orientation == "E")
                        object.second.transform.rotation = {0.f, 1.55f, 0.f};
                    else if (orientation == "S")
                        object.second.transform.rotation = {0.f, 3.14f, 0.f};
                    else if (orientation == "W")
                        object.second.transform.rotation = {0.f, -1.57f, 0.f};
                }
            }
        }
    }
}

void ZappyGui::eggLayingPose(int playerNumber)
{
    // change trantorians pose to show it is laying an egg
    std::cout << "Egg laying pose" << std::endl;
}

void ZappyGui::addEgg(
    int eggNumber, int playerNumber, const glm::vec3 &position)
{
    ZappyGameObject::id_t EggId =
        createGameObject(executablePath + "/ZappyGui/models/cube.obj",
            executablePath + "/ZappyGui/textures/egg.png", position,
            {0.f, 0.f, 0.f}, {0.15f, 0.15f, 0.15f}, true);
    Egg newEgg(EggId, eggNumber, playerNumber, position);
    this->eggs_.emplace_back(newEgg);
}

float ZappyGui::degreeToRadiant(float degree) { return degree * M_PI / 180; }

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
    for (float i = 0.f; i < width; i++) {
        createGameObject(executablePath + "/ZappyGui/models/Obsidian.obj",
            executablePath + "/ZappyGui/textures/Obsidian.png",
            {i, -3.f, -1.f}, {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}, true);
        createGameObject(executablePath + "/ZappyGui/models/Obsidian.obj",
            executablePath + "/ZappyGui/textures/Obsidian.png", {i, 0.f, -1.f},
            {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}, true);
        createGameObject(executablePath + "/ZappyGui/models/Obsidian.obj",
            executablePath + "/ZappyGui/textures/Obsidian.png",
            {i, 0.f, height}, {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}, true);
        createGameObject(executablePath + "/ZappyGui/models/Obsidian.obj",
            executablePath + "/ZappyGui/textures/Obsidian.png",
            {i, -3.f, height}, {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}, true);
        portalFrames.push_back(
            createGameObject(executablePath + "/ZappyGui/models/Portal.obj",
                executablePath + "/ZappyGui/textures/portal/frame1.png",
                {i, -1.5f, -1.f}, {degreeToRadiant(90), 0.f, 0.f},
                {1.f, 1.f, 1.f}, true));
        portalFrames.push_back(
            createGameObject(executablePath + "/ZappyGui/models/Portal.obj",
                executablePath + "/ZappyGui/textures/portal/frame1.png",
                {i, -0.5f, -1.f}, {degreeToRadiant(90), 0.f, 0.f},
                {1.f, 1.f, 1.f}, true));
        portalFrames.push_back(
            createGameObject(executablePath + "/ZappyGui/models/Portal.obj",
                executablePath + "/ZappyGui/textures/portal/frame1.png",
                {i, -1.5f, height}, {degreeToRadiant(90), 0.f, 0.f},
                {1.f, 1.f, 1.f}, true));
        portalFrames.push_back(
            createGameObject(executablePath + "/ZappyGui/models/Portal.obj",
                executablePath + "/ZappyGui/textures/portal/frame1.png",
                {i, -0.5f, height}, {degreeToRadiant(90), 0.f, 0.f},
                {1.f, 1.f, 1.f}, true));
    }
    for (float i = 0.f; i < height; i++) {
        createGameObject(executablePath + "/ZappyGui/models/Obsidian.obj",
            executablePath + "/ZappyGui/textures/Obsidian.png", {-1.f, 0.f, i},
            {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}, true);
        createGameObject(executablePath + "/ZappyGui/models/Obsidian.obj",
            executablePath + "/ZappyGui/textures/Obsidian.png",
            {-1.f, -3.f, i}, {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}, true);
        createGameObject(executablePath + "/ZappyGui/models/Obsidian.obj",
            executablePath + "/ZappyGui/textures/Obsidian.png",
            {width, 0.f, i}, {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}, true);
        createGameObject(executablePath + "/ZappyGui/models/Obsidian.obj",
            executablePath + "/ZappyGui/textures/Obsidian.png",
            {width, -3.f, i}, {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}, true);
        portalFrames.push_back(
            createGameObject(executablePath + "/ZappyGui/models/Portal.obj",
                executablePath + "/ZappyGui/textures/portal/frame1.png",
                {-1.f, -1.5f, i}, {0.f, 0.f, degreeToRadiant(90)},
                {1.f, 1.f, 1.f}, true));
        portalFrames.push_back(
            createGameObject(executablePath + "/ZappyGui/models/Portal.obj",
                executablePath + "/ZappyGui/textures/portal/frame1.png",
                {-1.f, -0.5f, i}, {0.f, 0.f, degreeToRadiant(90)},
                {1.f, 1.f, 1.f}, true));
        portalFrames.push_back(
            createGameObject(executablePath + "/ZappyGui/models/Portal.obj",
                executablePath + "/ZappyGui/textures/portal/frame1.png",
                {width, -1.5f, i}, {0.f, 0.f, degreeToRadiant(90)},
                {1.f, 1.f, 1.f}, true));
        portalFrames.push_back(
            createGameObject(executablePath + "/ZappyGui/models/Portal.obj",
                executablePath + "/ZappyGui/textures/portal/frame1.png",
                {width, -0.5f, i}, {0.f, 0.f, degreeToRadiant(90)},
                {1.f, 1.f, 1.f}, true));
    }
    for (float i = 0.f; i > -4.f; i--) {
        createGameObject(executablePath + "/ZappyGui/models/Obsidian.obj",
            executablePath + "/ZappyGui/textures/Obsidian.png",
            {-1.f, i, -1.f}, {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}, true);
        createGameObject(executablePath + "/ZappyGui/models/Obsidian.obj",
            executablePath + "/ZappyGui/textures/Obsidian.png",
            {-1.f, i, height}, {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}, true);
        createGameObject(executablePath + "/ZappyGui/models/Obsidian.obj",
            executablePath + "/ZappyGui/textures/Obsidian.png",
            {width, i, -1.f}, {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}, true);
        createGameObject(executablePath + "/ZappyGui/models/Obsidian.obj",
            executablePath + "/ZappyGui/textures/Obsidian.png",
            {width, i, height}, {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}, true);
    }
    createGameObject(executablePath + "/ZappyGui/models/SkySquare.obj",
        executablePath + "/ZappyGui/textures/SkyTop.png",
        {width / 2, -25.f, height / 2}, {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f},
        true);
    createGameObject(executablePath + "/ZappyGui/models/SkySquare.obj",
        executablePath + "/ZappyGui/textures/Sky.png",
        {width / 2, 25.f, height / 2}, {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}, true);
    createGameObject(executablePath + "/ZappyGui/models/SkySquare.obj",
        executablePath + "/ZappyGui/textures/Sky.png",
        {-25.f + (width / 2), 0.f, (height / 2)},
        {0.f, 0.f, degreeToRadiant(90)}, {1.f, 1.f, 1.f}, true);
    createGameObject(executablePath + "/ZappyGui/models/SkySquare.obj",
        executablePath + "/ZappyGui/textures/Sky.png",
        {25.f + (width / 2), 0.f, (height / 2)},
        {0.f, 0.f, degreeToRadiant(90)}, {1.f, 1.f, 1.f}, true);
    createGameObject(executablePath + "/ZappyGui/models/SkySquare.obj",
        executablePath + "/ZappyGui/textures/Sky.png",
        {(width / 2), 0.f, -25.f + (height / 2)},
        {degreeToRadiant(90), 0.f, 0.f}, {1.f, 1.f, 1.f}, true);
    createGameObject(executablePath + "/ZappyGui/models/SkySquare.obj",
        executablePath + "/ZappyGui/textures/Sky.png",
        {(width / 2), 0.f, 25.f + (height / 2)},
        {degreeToRadiant(90), 0.f, 0.f}, {1.f, 1.f, 1.f}, true);
    indexPortalFrame = 1;
}

void ZappyGui::removeGameObject(ZappyGameObject::id_t gameObjectId)
{
    vkDeviceWaitIdle(lveDevice.device());
    gameObjects.erase(gameObjectId);
}

float ZappyGui::getRandomFloat(float min, float max)
{
    return min +
        static_cast<float>(std::rand()) /
        (static_cast<float>(RAND_MAX / (max - min)));
}

void ZappyGui::updateGameObjectsTexture(
    std::string texturePath, ZappyGameObject::id_t gameObjectId)
{
    int index = 0;
    gameObjects[gameObjectId].hasDescriptorSet = true;
    for (std::pair<std::vector<VkDescriptorSet>,
             std::pair<std::shared_ptr<zappy::Texture>, std::string>>
             &textureObject : textureObjects) {
        if (textureObject.second.second == texturePath) {
            gameObjects[gameObjectId].indexDescriptorSet = index;
            return;
        }
        index++;
    }

    std::shared_ptr<Texture> texture =
        std::make_shared<Texture>(lveDevice, texturePath);
    gameObjects[gameObjectId].imageInfo.sampler = texture->getSampler();
    gameObjects[gameObjectId].imageInfo.imageView = texture->getImageView();
    gameObjects[gameObjectId].imageInfo.imageLayout =
        texture->getImageLayout();

    std::vector<VkDescriptorSet> descriptorSets(
        ZappySwapChain::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < uboBuffers.size(); i++) {
        auto bufferInfo = uboBuffers[i]->descriptorInfo();
        ZappyDescriptorWriter(*globalSetLayout, *globalPool)
            .writeBuffer(0, &bufferInfo)
            .writeImage(1, &gameObjects[gameObjectId].imageInfo)
            .build(descriptorSets[i]);
    }
    textureObjects.push_back(
        std::make_pair(descriptorSets, std::make_pair(texture, texturePath)));
    gameObjects[gameObjectId].indexDescriptorSet = textureObjects.size() - 1;
}

void ZappyGui::updateGame()
{
    if (timerManager_.getElapsedTime("portalAnimation") > 0.05f) {
        updatePortalFrame();
        timerManager_.resetTimer("portalAnimation");
    }
    if (timerManager_.getElapsedTime("resourcesAnimation") > 0.05f) {
        updateResourcesAnimation();
        timerManager_.resetTimer("resourcesAnimation");
    }
    for (Trantorian &Trantorian : trantorians_) {
        if (Trantorian.incatationInProgess) {
            for (auto &object : gameObjects) {
                if (object.first == Trantorian.trantorianObject) {
                    object.second.transform.rotation.y += 0.01f;
                }
            }
        }
    }
}

void ZappyGui::updateResourcesAnimation()
{
    for (ZappyGameObject::id_t linemateId : resources_["linemate"]) {
        gameObjects[linemateId].transform.rotation.y += 0.02f;
        if (indexRessourcesAnimation == 1)
            gameObjects[linemateId].transform.translation.y -= 0.01f;
        else
            gameObjects[linemateId].transform.translation.y += 0.01f;
    }
    for (ZappyGameObject::id_t deraumereId : resources_["deraumere"]) {
        gameObjects[deraumereId].transform.rotation.y += 0.02f;
        if (indexRessourcesAnimation == 1)
            gameObjects[deraumereId].transform.translation.y -= 0.01f;
        else
            gameObjects[deraumereId].transform.translation.y += 0.01f;
    }
    for (ZappyGameObject::id_t siburId : resources_["sibur"]) {
        gameObjects[siburId].transform.rotation.y += 0.02f;
        if (indexRessourcesAnimation == 1)
            gameObjects[siburId].transform.translation.y -= 0.01f;
        else
            gameObjects[siburId].transform.translation.y += 0.01f;
    }
    for (ZappyGameObject::id_t mendianeId : resources_["mendiane"]) {
        gameObjects[mendianeId].transform.rotation.y += 0.02f;
        if (indexRessourcesAnimation == 1)
            gameObjects[mendianeId].transform.translation.y -= 0.01f;
        else
            gameObjects[mendianeId].transform.translation.y += 0.01f;
    }
    for (ZappyGameObject::id_t phirasId : resources_["phiras"]) {
        gameObjects[phirasId].transform.rotation.y += 0.02f;
        if (indexRessourcesAnimation == 1)
            gameObjects[phirasId].transform.translation.y -= 0.01f;
        else
            gameObjects[phirasId].transform.translation.y += 0.01f;
    }
    for (ZappyGameObject::id_t thystameId : resources_["thystame"]) {
        gameObjects[thystameId].transform.rotation.y += 0.02f;
        if (indexRessourcesAnimation == 1)
            gameObjects[thystameId].transform.translation.y -= 0.01f;
        else
            gameObjects[thystameId].transform.translation.y += 0.01f;
    }
    for (ZappyGameObject::id_t foodId : resources_["food"]) {
        gameObjects[foodId].transform.rotation.y += 0.02f;
        if (indexRessourcesAnimation == 1)
            gameObjects[foodId].transform.translation.y -= 0.01f;
        else
            gameObjects[foodId].transform.translation.y += 0.01f;
    }
    if (resources_["food"].empty())
        return;
    if (gameObjects[resources_["food"][0]].transform.translation.y < -0.07f)
        indexRessourcesAnimation = 0;
    if (gameObjects[resources_["food"][0]].transform.translation.y > 0.02f)
        indexRessourcesAnimation = 1;
}

void ZappyGui::updatePortalFrame()
{
    if (indexPortalFrame == 32)
        indexPortalFrame = 1;
    else
        indexPortalFrame++;
    for (ZappyGameObject::id_t portalFrame : portalFrames) {
        updateGameObjectsTexture(executablePath +
                "/ZappyGui/textures/portal/frame" +
                std::to_string(indexPortalFrame) + ".png",
            portalFrame);
    }
}

} // namespace zappy
