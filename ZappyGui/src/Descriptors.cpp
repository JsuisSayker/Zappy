/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Descriptors
*/

#include "Descriptors.hpp"
#include "ErrorHandling.hpp"

// std
#include <cassert>
#include <stdexcept>

namespace zappy {

// *************** Descriptor Set Layout Builder *********************


/**
 * @brief Adds a binding to the ZappyDescriptorSetLayout builder.
 * 
 * This function adds a binding to the ZappyDescriptorSetLayout builder. A binding consists of a binding index, 
 * descriptor type, shader stage flags, and count. The binding index must be unique within the builder. 
 * The descriptor type specifies the type of the descriptor, such as uniform buffer, storage buffer, or sampler. 
 * The shader stage flags specify the shader stages that can access the descriptor. 
 * The count specifies the number of descriptors in the binding.
 * 
 * @param binding The binding index.
 * @param descriptorType The type of the descriptor.
 * @param stageFlags The shader stage flags.
 * @param count The number of descriptors in the binding.
 * @return A reference to the ZappyDescriptorSetLayout builder.
 */
ZappyDescriptorSetLayout::Builder &
ZappyDescriptorSetLayout::Builder::addBinding(uint32_t binding,
    VkDescriptorType descriptorType, VkShaderStageFlags stageFlags,
    uint32_t count)
{
    assert(bindings.count(binding) == 0 && "Binding already in use");
    VkDescriptorSetLayoutBinding layoutBinding{};
    layoutBinding.binding = binding;
    layoutBinding.descriptorType = descriptorType;
    layoutBinding.descriptorCount = count;
    layoutBinding.stageFlags = stageFlags;
    bindings[binding] = layoutBinding;
    return *this;
}


/**
 * @brief Builds a unique pointer to a ZappyDescriptorSetLayout object.
 *
 * This function constructs a unique pointer to a ZappyDescriptorSetLayout object using the provided
 * zappyDevice and bindings. The constructed object is then returned as a unique pointer.
 *
 * @return A unique pointer to a ZappyDescriptorSetLayout object.
 */
std::unique_ptr<ZappyDescriptorSetLayout>
ZappyDescriptorSetLayout::Builder::build() const
{
    return std::make_unique<ZappyDescriptorSetLayout>(zappyDevice, bindings);
}

// *************** Descriptor Set Layout *********************


/**
 * @brief Constructs a ZappyDescriptorSetLayout object.
 *
 * This constructor initializes a ZappyDescriptorSetLayout object with the specified device and descriptor set layout bindings.
 *
 * @param zappyDevice The ZappyDevice object associated with the descriptor set layout.
 * @param bindings A std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> containing the descriptor set layout bindings.
 */
ZappyDescriptorSetLayout::ZappyDescriptorSetLayout(ZappyDevice &zappyDevice,
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings)
    : zappyDevice{zappyDevice}, bindings{bindings}
{
    std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
    for (auto kv : bindings) {
        setLayoutBindings.push_back(kv.second);
    }

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
    descriptorSetLayoutInfo.sType =
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutInfo.bindingCount =
        static_cast<uint32_t>(setLayoutBindings.size());
    descriptorSetLayoutInfo.pBindings = setLayoutBindings.data();

    if (vkCreateDescriptorSetLayout(zappyDevice.device(),
            &descriptorSetLayoutInfo, nullptr,
            &descriptorSetLayout) != VK_SUCCESS) {
        throw zappy::DescriptorSetLayoutCreationFailedException();
    }
}

/**
 * @brief Destroys the ZappyDescriptorSetLayout object.
 *
 */
ZappyDescriptorSetLayout::~ZappyDescriptorSetLayout()
{
    vkDestroyDescriptorSetLayout(
        zappyDevice.device(), descriptorSetLayout, nullptr);
}

// *************** Descriptor Pool Builder *********************

/**
 * @brief Adds a pool size to the ZappyDescriptorPool builder.
 *
 * This function adds a pool size to the ZappyDescriptorPool builder. A pool size consists of a descriptor type and count.
 * The descriptor type specifies the type of the descriptor, such as uniform buffer, storage buffer, or sampler. 
 * The count specifies the number of descriptors of the specified type.
 *
 * @param descriptorType The type of the descriptor.
 * @param count The number of descriptors of the specified type.
 * @return A reference to the ZappyDescriptorPool builder.
 */
ZappyDescriptorPool::Builder &ZappyDescriptorPool::Builder::addPoolSize(
    VkDescriptorType descriptorType, uint32_t count)
{
    poolSizes.push_back({descriptorType, count});
    return *this;
}

/**
 * @brief Sets the pool flags for the ZappyDescriptorPool builder.
 *
 * This function sets the pool flags for the ZappyDescriptorPool builder. The pool flags specify the behavior of the descriptor pool.
 *
 * @param flags The pool flags.
 * @return A reference to the ZappyDescriptorPool builder.
 */
ZappyDescriptorPool::Builder &ZappyDescriptorPool::Builder::setPoolFlags(
    VkDescriptorPoolCreateFlags flags)
{
    poolFlags = flags;
    return *this;
}

/**
 * @brief Sets the maximum number of descriptor sets for the ZappyDescriptorPool builder.
 *
 * This function sets the maximum number of descriptor sets for the ZappyDescriptorPool builder. The maximum number of descriptor sets specifies the maximum number of descriptor sets that can be allocated from the pool.
 *
 * @param count The maximum number of descriptor sets.
 * @return A reference to the ZappyDescriptorPool builder.
 */
ZappyDescriptorPool::Builder &ZappyDescriptorPool::Builder::setMaxSets(
    uint32_t count)
{
    maxSets = count;
    return *this;
}

/**
 * @brief Builds a unique pointer to a ZappyDescriptorPool object.
 *
 * This function constructs a unique pointer to a ZappyDescriptorPool object using the provided
 * zappyDevice, maxSets, poolFlags, and poolSizes. The constructed object is then returned as a unique pointer.
 *
 * @return A unique pointer to a ZappyDescriptorPool object.
 */
std::unique_ptr<ZappyDescriptorPool>
ZappyDescriptorPool::Builder::build() const
{
    return std::make_unique<ZappyDescriptorPool>(
        zappyDevice, maxSets, poolFlags, poolSizes);
}


/**
 * @brief Constructs a ZappyDescriptorPool object.
 *
 * This constructor initializes a ZappyDescriptorPool object with the specified parameters.
 *
 * @param zappyDevice The ZappyDevice object associated with the descriptor pool.
 * @param maxSets The maximum number of descriptor sets that can be allocated from the pool.
 * @param poolFlags Flags controlling the behavior of the descriptor pool.
 * @param poolSizes A vector of VkDescriptorPoolSize objects specifying the number of descriptors of each type to allocate.
 *
 * @throws zappy::DescriptorPoolCreationFailedException if the descriptor pool creation fails.
 */
ZappyDescriptorPool::ZappyDescriptorPool(ZappyDevice &zappyDevice,
    uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags,
    const std::vector<VkDescriptorPoolSize> &poolSizes)
    : zappyDevice{zappyDevice}
{
    VkDescriptorPoolSize pool_sizes[] = {{VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
        {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
        {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
        {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
        {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
        {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
        {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
        {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
        {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};

    VkDescriptorPoolCreateInfo descriptorPoolInfo{};
    descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolInfo.poolSizeCount = std::size(pool_sizes);
    descriptorPoolInfo.pPoolSizes = pool_sizes;
    descriptorPoolInfo.maxSets = maxSets;
    descriptorPoolInfo.flags =
        poolFlags | VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

    if (vkCreateDescriptorPool(zappyDevice.device(), &descriptorPoolInfo,
            nullptr, &descriptorPool) != VK_SUCCESS) {
        throw zappy::DescriptorPoolCreationFailedException();
    }
}

/**
 * @brief Destroys the ZappyDescriptorPool object.
 *
 */
ZappyDescriptorPool::~ZappyDescriptorPool()
{
    vkDestroyDescriptorPool(zappyDevice.device(), descriptorPool, nullptr);
}

/**
 * @brief Allocates a descriptor set from the descriptor pool.
 *
 * This function allocates a descriptor set from the descriptor pool using the specified descriptor set layout.
 *
 * @param descriptorSetLayout The descriptor set layout.
 * @param descriptor The allocated descriptor set.
 * @return true if the descriptor set is successfully allocated, false otherwise.
 */
bool ZappyDescriptorPool::allocateDescriptor(
    const VkDescriptorSetLayout descriptorSetLayout,
    VkDescriptorSet &descriptor) const
{
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.pSetLayouts = &descriptorSetLayout;
    allocInfo.descriptorSetCount = 1;

    // Might want to create a "DescriptorPoolManager" class that handles this
    // case, and builds a new pool whenever an old pool fills up. But this is
    // beyond our current scope
    if (vkAllocateDescriptorSets(
            zappyDevice.device(), &allocInfo, &descriptor) != VK_SUCCESS) {
        return false;
    }
    return true;
}


/**
 * @brief Frees the specified Vulkan descriptor sets.
 *
 * This function frees the specified Vulkan descriptor sets from the descriptor pool.
 *
 * @param descriptors The vector of Vulkan descriptor sets to be freed.
 */
void ZappyDescriptorPool::freeDescriptors(
    std::vector<VkDescriptorSet> &descriptors) const
{
    vkFreeDescriptorSets(zappyDevice.device(), descriptorPool,
        static_cast<uint32_t>(descriptors.size()), descriptors.data());
}

/**
 * @brief Resets the descriptor pool.
 *
 * This function resets the descriptor pool.
 */
void ZappyDescriptorPool::resetPool()
{
    vkResetDescriptorPool(zappyDevice.device(), descriptorPool, 0);
}

// *************** Descriptor Writer *********************

/**
 * @brief Constructs a ZappyDescriptorWriter object.
 *
 * This constructor initializes a ZappyDescriptorWriter object with the specified parameters.
 *
 * @param setLayout The ZappyDescriptorSetLayout object associated with the descriptor writer.
 * @param pool The ZappyDescriptorPool object associated with the descriptor writer.
 */
ZappyDescriptorWriter::ZappyDescriptorWriter(
    ZappyDescriptorSetLayout &setLayout, ZappyDescriptorPool &pool)
    : setLayout{setLayout}, pool{pool}
{
}

/**
 * @brief Writes a buffer to the descriptor set.
 *
 * This function writes a buffer to the descriptor set using the specified binding and buffer info.
 *
 * @param binding The binding index.
 * @param bufferInfo The buffer info.
 * @return A reference to the ZappyDescriptorWriter.
 */
ZappyDescriptorWriter &ZappyDescriptorWriter::writeBuffer(
    uint32_t binding, VkDescriptorBufferInfo *bufferInfo)
{
    assert(setLayout.bindings.count(binding) == 1 &&
        "Layout does not contain specified binding");

    auto &bindingDescription = setLayout.bindings[binding];

    assert(bindingDescription.descriptorCount == 1 &&
        "Binding single descriptor info, but binding expects multiple");

    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.descriptorType = bindingDescription.descriptorType;
    write.dstBinding = binding;
    write.pBufferInfo = bufferInfo;
    write.descriptorCount = 1;

    writes.push_back(write);
    return *this;
}


ZappyDescriptorWriter &ZappyDescriptorWriter::writeImage(
    uint32_t binding, VkDescriptorImageInfo *imageInfo)
{
    assert(setLayout.bindings.count(binding) == 1 &&
        "Layout does not contain specified binding");

    auto &bindingDescription = setLayout.bindings[binding];

    assert(bindingDescription.descriptorCount == 1 &&
        "Binding single descriptor info, but binding expects multiple");

    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.descriptorType = bindingDescription.descriptorType;
    write.dstBinding = binding;
    write.pImageInfo = imageInfo;
    write.descriptorCount = 1;

    writes.push_back(write);
    return *this;
}


/**
 * @brief Builds a Vulkan descriptor set using the specified descriptor set layout.
 *
 * This function allocates a descriptor set from the descriptor pool using the provided descriptor set layout.
 * It then overwrites the descriptor set with the necessary data.
 *
 * @param set The reference to the Vulkan descriptor set that will be built.
 * @return Returns true if the descriptor set is successfully built, false otherwise.
 */
bool ZappyDescriptorWriter::build(VkDescriptorSet &set)
{
    bool success =
        pool.allocateDescriptor(setLayout.getDescriptorSetLayout(), set);
    if (!success) {
        return false;
    }
    overwrite(set);
    return true;
}

/**
 * @brief Overwrites the specified Vulkan descriptor set.
 *
 * This function overwrites the specified Vulkan descriptor set with the necessary data.
 *
 * @param set The reference to the Vulkan descriptor set that will be overwritten.
 */
void ZappyDescriptorWriter::overwrite(VkDescriptorSet &set)
{
    for (auto &write : writes) {
        write.dstSet = set;
    }
    vkUpdateDescriptorSets(
        pool.zappyDevice.device(), writes.size(), writes.data(), 0, nullptr);
}

} // namespace zappy
