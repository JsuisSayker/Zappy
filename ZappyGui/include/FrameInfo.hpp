/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** FrameInfo
*/

#pragma once

#include "Camera.hpp"
#include "GameObject.hpp"
#include "Texture.hpp"

// lib
#include <vulkan/vulkan.h>
#include <memory>

namespace zappy {

#define MAX_LIGHTS 10

struct PointLight {
    glm::vec4 position{}; // ignore w
    glm::vec4 color{};    // w is intensity
};

struct GlobalUbo {
    glm::mat4 projection{1.f};
    glm::mat4 view{1.f};
    glm::mat4 inverseView{1.f};
    glm::vec4 ambientLightColor{1.f, 1.f, 1.f, .2f}; // w is intensity
    PointLight pointLights[MAX_LIGHTS];
    int numLights;
};

struct FrameInfo {
    int frameIndex;
    float frameTime;
    VkCommandBuffer commandBuffer;
    ZappyCamera &camera;
    VkDescriptorSet globalDescriptorSet;
    std::vector<std::pair<std::vector<VkDescriptorSet>, std::pair<std::shared_ptr<Texture>, std::string>>> textureObjects;
    ZappyGameObject::Map &gameObjects;
};
} // namespace zappy
