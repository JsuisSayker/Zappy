/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** GameObject
*/

#pragma once

#include "Model.hpp"
#include "Texture.hpp"

// libs
#include <glm/gtc/matrix_transform.hpp>

// std
#include <memory>
#include <unordered_map>

namespace zappy {

struct TransformComponent {
    glm::vec3 translation{};
    glm::vec3 scale{1.f, 1.f, 1.f};
    glm::vec3 rotation{};

    // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
    // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
    // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
    glm::mat4 mat4();

    glm::mat3 normalMatrix();
};

struct PointLightComponent {
    float lightIntensity = 1.0f;
};

class ZappyGameObject {
  public:
    using id_t = unsigned int;
    using Map = std::unordered_map<id_t, ZappyGameObject>;

    static ZappyGameObject createGameObject()
    {
        static id_t currentId = 0;
        return ZappyGameObject{currentId++};
    }

    static ZappyGameObject makePointLight(float intensity = 10.f,
        float radius = 0.1f, glm::vec3 color = glm::vec3(1.f));

    ZappyGameObject(const ZappyGameObject &) = delete;
    ZappyGameObject &operator=(const ZappyGameObject &) = delete;
    ZappyGameObject(ZappyGameObject &&) = default;
    ZappyGameObject &operator=(ZappyGameObject &&) = default;

    id_t getId() { return id; }

    glm::vec3 color{};
    TransformComponent transform{};

    std::vector<VkDescriptorSet> descriptorSets;
    int indexDescriptorSet;
    bool hasDescriptorSet{false};

    VkDescriptorImageInfo imageInfo{};

    // Optional pointer components
    std::shared_ptr<ZappyModel> model = nullptr;
    std::unique_ptr<PointLightComponent> pointLight = nullptr;

  private:
    ZappyGameObject(id_t objId) : id{objId} {}

    id_t id;
};
} // namespace zappy
