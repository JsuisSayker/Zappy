/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Model
*/

#include "Model.hpp"
#include "ErrorHandling.hpp"
#include "Utils.hpp"

// libs
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

// std
#include <cassert>
#include <cstring>
#include <unordered_map>
#include <iostream>

#ifndef ENGINE_DIR
#define ENGINE_DIR ""
#endif

namespace std {
template <>
struct hash<zappy::ZappyModel::Vertex> {
  size_t operator()(zappy::ZappyModel::Vertex const &vertex) const {
    size_t seed = 0;
    zappy::hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
    return seed;
  }
};
}  // namespace std

namespace zappy {


/**
 * @brief Constructs a new ZappyModel object.
 *
 * This constructor initializes a new instance of the ZappyModel class.
 *
 * @param device The ZappyDevice object associated with the model.
 * @param builder The Builder object containing the vertex and index data for the model.
 */
ZappyModel::ZappyModel(ZappyDevice &device, const ZappyModel::Builder &builder) : zappyDevice{device} {
  createVertexBuffers(builder.vertices);
  createIndexBuffers(builder.indices);
}

/**
 * @brief Destroys the ZappyModel object.
 *
 */
ZappyModel::~ZappyModel() {}

/**
 * @brief Creates a new ZappyModel object from a file.
 *
 * This function creates a new ZappyModel object from a file.
 *
 * @param device The ZappyDevice object associated with the model.
 * @param filepath The path to the file containing the model data.
 * @return std::unique_ptr<ZappyModel> A unique pointer to the newly created ZappyModel object.
 */
std::unique_ptr<ZappyModel> ZappyModel::createModelFromFile(
    ZappyDevice &device, const std::string &filepath) {
  Builder builder{};
  builder.loadModel(ENGINE_DIR + filepath);
  return std::make_unique<ZappyModel>(device, builder);
}

/**
 * @brief Creates the vertex buffers.
 *
 * This function creates the vertex buffers for the model.
 *
 * @param vertices The vertices of the model.
 */
void ZappyModel::createVertexBuffers(const std::vector<Vertex> &vertices) {
  vertexCount = static_cast<uint32_t>(vertices.size());
  assert(vertexCount >= 3 && "Vertex count must be at least 3");
  VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
  uint32_t vertexSize = sizeof(vertices[0]);

  ZappyBuffer stagingBuffer{
      zappyDevice,
      vertexSize,
      vertexCount,
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
  };

  stagingBuffer.map();
  stagingBuffer.writeToBuffer((void *)vertices.data());

  vertexBuffer = std::make_unique<ZappyBuffer>(
      zappyDevice,
      vertexSize,
      vertexCount,
      VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  zappyDevice.copyBuffer(stagingBuffer.getBuffer(), vertexBuffer->getBuffer(), bufferSize);
}

/**
 * @brief Creates the index buffers.
 *
 * This function creates the index buffers for the model.
 *
 * @param indices The indices of the model.
 */
void ZappyModel::createIndexBuffers(const std::vector<uint32_t> &indices) {
  indexCount = static_cast<uint32_t>(indices.size());
  hasIndexBuffer = indexCount > 0;

  if (!hasIndexBuffer) {
    return;
  }

  VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;
  uint32_t indexSize = sizeof(indices[0]);

  ZappyBuffer stagingBuffer{
      zappyDevice,
      indexSize,
      indexCount,
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
  };

  stagingBuffer.map();
  stagingBuffer.writeToBuffer((void *)indices.data());

  indexBuffer = std::make_unique<ZappyBuffer>(
      zappyDevice,
      indexSize,
      indexCount,
      VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  zappyDevice.copyBuffer(stagingBuffer.getBuffer(), indexBuffer->getBuffer(), bufferSize);
}

/**
 * @brief Draws the model.
 *
 * This function draws the model.
 *
 * @param commandBuffer The command buffer.
 */
void ZappyModel::draw(VkCommandBuffer commandBuffer) {
  if (hasIndexBuffer) {
    vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
  } else {
    vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
  }
}

/**
 * @brief Binds the model.
 *
 * This function binds the model.
 *
 * @param commandBuffer The command buffer.
 */
void ZappyModel::bind(VkCommandBuffer commandBuffer) {
  VkBuffer buffers[] = {vertexBuffer->getBuffer()};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

  if (hasIndexBuffer) {
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
  }
}


/**
 * Returns the binding descriptions for the vertex input of the ZappyModel::Vertex class.
 * 
 * @return A vector of VkVertexInputBindingDescription objects representing the binding descriptions.
 */
std::vector<VkVertexInputBindingDescription> ZappyModel::Vertex::getBindingDescriptions() {
  std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
  bindingDescriptions[0].binding = 0;
  bindingDescriptions[0].stride = sizeof(Vertex);
  bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  return bindingDescriptions;
}


/**
 * Returns the attribute descriptions for the vertex.
 *
 * @return A vector of VkVertexInputAttributeDescription objects representing the attribute descriptions.
 */
std::vector<VkVertexInputAttributeDescription> ZappyModel::Vertex::getAttributeDescriptions() {
  std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

  attributeDescriptions.push_back({0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position)});
  attributeDescriptions.push_back({1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)});
  attributeDescriptions.push_back({2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal)});
  attributeDescriptions.push_back({3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv)});

  return attributeDescriptions;
}


/**
 * Loads a 3D model from the specified file path.
 *
 * @param filepath The path to the model file.
 * @throws zappy::LoadObjFailedException if the model file fails to load.
 */
void ZappyModel::Builder::loadModel(const std::string &filepath) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string warn, err;

  if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) {
    std::cout << "Error: " << warn + err << std::endl;
    throw zappy::LoadObjFailedException();
  }

  vertices.clear();
  indices.clear();

  std::unordered_map<Vertex, uint32_t> uniqueVertices{};
  for (const auto &shape : shapes) {
    for (const auto &index : shape.mesh.indices) {
      Vertex vertex{};

      if (index.vertex_index >= 0) {
        vertex.position = {
            attrib.vertices[3 * index.vertex_index + 0],
            attrib.vertices[3 * index.vertex_index + 1],
            attrib.vertices[3 * index.vertex_index + 2],
        };

        vertex.color = {
            attrib.colors[3 * index.vertex_index + 0],
            attrib.colors[3 * index.vertex_index + 1],
            attrib.colors[3 * index.vertex_index + 2],
        };
      }

      if (index.normal_index >= 0) {
        vertex.normal = {
            attrib.normals[3 * index.normal_index + 0],
            attrib.normals[3 * index.normal_index + 1],
            attrib.normals[3 * index.normal_index + 2],
        };
      }

      if (index.texcoord_index >= 0) {
        vertex.uv = {
            attrib.texcoords[2 * index.texcoord_index + 0],
            attrib.texcoords[2 * index.texcoord_index + 1],
        };
      }

      if (uniqueVertices.count(vertex) == 0) {
        uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
        vertices.push_back(vertex);
      }
      indices.push_back(uniqueVertices[vertex]);
    }
  }
}

}  // namespace zappy
