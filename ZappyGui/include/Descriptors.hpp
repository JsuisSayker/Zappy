/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Descriptors
*/

#pragma once

#include "Device.hpp"

// std
#include <memory>
#include <unordered_map>
#include <vector>

namespace zappy {

class ZappyDescriptorSetLayout {
  public:
    class Builder {
      public:
        Builder(ZappyDevice &zappyDevice) : zappyDevice{zappyDevice} {}

        Builder &addBinding(uint32_t binding, VkDescriptorType descriptorType,
            VkShaderStageFlags stageFlags, uint32_t count = 1);
        std::unique_ptr<ZappyDescriptorSetLayout> build() const;

      private:
        ZappyDevice &zappyDevice;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
    };

    ZappyDescriptorSetLayout(ZappyDevice &zappyDevice,
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
    ~ZappyDescriptorSetLayout();
    ZappyDescriptorSetLayout(const ZappyDescriptorSetLayout &) = delete;
    ZappyDescriptorSetLayout &operator=(
        const ZappyDescriptorSetLayout &) = delete;

    VkDescriptorSetLayout getDescriptorSetLayout() const
    {
        return descriptorSetLayout;
    }

  private:
    ZappyDevice &zappyDevice;
    VkDescriptorSetLayout descriptorSetLayout;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

    friend class ZappyDescriptorWriter;
};

class ZappyDescriptorPool {
  public:
    class Builder {
      public:
        Builder(ZappyDevice &zappyDevice) : zappyDevice{zappyDevice} {}

        Builder &addPoolSize(VkDescriptorType descriptorType, uint32_t count);
        Builder &setPoolFlags(VkDescriptorPoolCreateFlags flags);
        Builder &setMaxSets(uint32_t count);
        std::unique_ptr<ZappyDescriptorPool> build() const;

      private:
        ZappyDevice &zappyDevice;
        std::vector<VkDescriptorPoolSize> poolSizes{};
        uint32_t maxSets = 1000;
        VkDescriptorPoolCreateFlags poolFlags = 0;
    };

    ZappyDescriptorPool(ZappyDevice &zappyDevice, uint32_t maxSets,
        VkDescriptorPoolCreateFlags poolFlags,
        const std::vector<VkDescriptorPoolSize> &poolSizes);
    ~ZappyDescriptorPool();
    ZappyDescriptorPool(const ZappyDescriptorPool &) = delete;
    ZappyDescriptorPool &operator=(const ZappyDescriptorPool &) = delete;

    bool allocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout,
        VkDescriptorSet &descriptor) const;

    void freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;

    void resetPool();

    VkDescriptorPool descriptorPool;
  private:
    ZappyDevice &zappyDevice;

    friend class ZappyDescriptorWriter;
};

class ZappyDescriptorWriter {
  public:
    ZappyDescriptorWriter(
        ZappyDescriptorSetLayout &setLayout, ZappyDescriptorPool &pool);

    ZappyDescriptorWriter &writeBuffer(
        uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
    ZappyDescriptorWriter &writeImage(
        uint32_t binding, VkDescriptorImageInfo *imageInfo);

    bool build(VkDescriptorSet &set);
    void overwrite(VkDescriptorSet &set);

  private:
    ZappyDescriptorSetLayout &setLayout;
    ZappyDescriptorPool &pool;
    std::vector<VkWriteDescriptorSet> writes;
};

} // namespace zappy
