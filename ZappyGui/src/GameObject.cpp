/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** GameObject
*/

#include "GameObject.hpp"

namespace zappy {


/**
 * Calculates and returns the transformation matrix for the TransformComponent.
 * The transformation matrix is a 4x4 matrix that represents the translation, rotation, and scale of the object.
 *
 * @return The transformation matrix as a glm::mat4.
 */
glm::mat4 TransformComponent::mat4()
{
    const float c3 = glm::cos(rotation.z);
    const float s3 = glm::sin(rotation.z);
    const float c2 = glm::cos(rotation.x);
    const float s2 = glm::sin(rotation.x);
    const float c1 = glm::cos(rotation.y);
    const float s1 = glm::sin(rotation.y);
    return glm::mat4{{
                         scale.x * (c1 * c3 + s1 * s2 * s3),
                         scale.x * (c2 * s3),
                         scale.x * (c1 * s2 * s3 - c3 * s1),
                         0.0f,
                     },
        {
            scale.y * (c3 * s1 * s2 - c1 * s3),
            scale.y * (c2 * c3),
            scale.y * (c1 * c3 * s2 + s1 * s3),
            0.0f,
        },
        {
            scale.z * (c2 * s1),
            scale.z * (-s2),
            scale.z * (c1 * c2),
            0.0f,
        },
        {translation.x, translation.y, translation.z, 1.0f}};
}


/**
 * Calculates the normal matrix for the current transformation.
 * The normal matrix is used to transform normal vectors from object space to eye space.
 *
 * @return The normal matrix.
 */
glm::mat3 TransformComponent::normalMatrix()
{
    const float c3 = glm::cos(rotation.z);
    const float s3 = glm::sin(rotation.z);
    const float c2 = glm::cos(rotation.x);
    const float s2 = glm::sin(rotation.x);
    const float c1 = glm::cos(rotation.y);
    const float s1 = glm::sin(rotation.y);
    const glm::vec3 invScale = 1.0f / scale;

    return glm::mat3{
        {
            invScale.x * (c1 * c3 + s1 * s2 * s3),
            invScale.x * (c2 * s3),
            invScale.x * (c1 * s2 * s3 - c3 * s1),
        },
        {
            invScale.y * (c3 * s1 * s2 - c1 * s3),
            invScale.y * (c2 * c3),
            invScale.y * (c1 * c3 * s2 + s1 * s3),
        },
        {
            invScale.z * (c2 * s1),
            invScale.z * (-s2),
            invScale.z * (c1 * c2),
        },
    };
}


/**
 * Creates a point light game object with the specified intensity, radius, and color.
 *
 * @param intensity The intensity of the point light.
 * @param radius The radius of the point light.
 * @param color The color of the point light.
 * @return The created ZappyGameObject with the specified properties.
 */
ZappyGameObject ZappyGameObject::makePointLight(
    float intensity, float radius, glm::vec3 color)
{
    ZappyGameObject gameObj = ZappyGameObject::createGameObject();
    gameObj.color = color;
    gameObj.transform.scale.x = radius;
    gameObj.pointLight = std::make_unique<PointLightComponent>();
    gameObj.pointLight->lightIntensity = intensity;
    return gameObj;
}

} // namespace zappy
