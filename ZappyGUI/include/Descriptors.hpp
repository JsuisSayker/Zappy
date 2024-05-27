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
    Builder(ZappyDevice &lveDevice) : lveDevice{lveDevice} {}

    Builder &addBinding(
        uint32_t binding,
        VkDescriptorType descriptorType,
        VkShaderStageFlags stageFlags,
        uint32_t count = 1);
    std::unique_ptr<ZappyDescriptorSetLayout> build() const;

   private:
    ZappyDevice &lveDevice;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
  };

  ZappyDescriptorSetLayout(
      ZappyDevice &lveDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
  ~ZappyDescriptorSetLayout();
  ZappyDescriptorSetLayout(const ZappyDescriptorSetLayout &) = delete;
  ZappyDescriptorSetLayout &operator=(const ZappyDescriptorSetLayout &) = delete;

  VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

 private:
  ZappyDevice &lveDevice;
  VkDescriptorSetLayout descriptorSetLayout;
  std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

  friend class LveDescriptorWriter;
};

class LveDescriptorPool {
 public:
  class Builder {
   public:
    Builder(ZappyDevice &lveDevice) : lveDevice{lveDevice} {}

    Builder &addPoolSize(VkDescriptorType descriptorType, uint32_t count);
    Builder &setPoolFlags(VkDescriptorPoolCreateFlags flags);
    Builder &setMaxSets(uint32_t count);
    std::unique_ptr<LveDescriptorPool> build() const;

   private:
    ZappyDevice &lveDevice;
    std::vector<VkDescriptorPoolSize> poolSizes{};
    uint32_t maxSets = 1000;
    VkDescriptorPoolCreateFlags poolFlags = 0;
  };

  LveDescriptorPool(
      ZappyDevice &lveDevice,
      uint32_t maxSets,
      VkDescriptorPoolCreateFlags poolFlags,
      const std::vector<VkDescriptorPoolSize> &poolSizes);
  ~LveDescriptorPool();
  LveDescriptorPool(const LveDescriptorPool &) = delete;
  LveDescriptorPool &operator=(const LveDescriptorPool &) = delete;

  bool allocateDescriptor(
      const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const;

  void freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;

  void resetPool();

 private:
  ZappyDevice &lveDevice;
  VkDescriptorPool descriptorPool;

  friend class LveDescriptorWriter;
};

class LveDescriptorWriter {
 public:
  LveDescriptorWriter(ZappyDescriptorSetLayout &setLayout, LveDescriptorPool &pool);

  LveDescriptorWriter &writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
  LveDescriptorWriter &writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);

  bool build(VkDescriptorSet &set);
  void overwrite(VkDescriptorSet &set);

 private:
  ZappyDescriptorSetLayout &setLayout;
  LveDescriptorPool &pool;
  std::vector<VkWriteDescriptorSet> writes;
};

}  // namespace zappy
