/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Device
*/

#include "Device.hpp"
#include "ErrorHandling.hpp"

// std headers
#include <cstring>
#include <iostream>
#include <set>
#include <unordered_set>

namespace zappy {


/**
 * @brief This function is a debug callback for Vulkan API.
 *
 * @param flags The flags that indicate the type of message.
 * @param objectType The type of the object that triggered the message.
 * @param object The handle of the object that triggered the message.
 * @param location The location of the message in the code.
 * @param messageCode The code of the message.
 * @param pLayerPrefix The prefix of the layer that triggered the message.
 * @param pMessage The message string.
 * @param pUserData User data provided during the callback registration.
 * @return VkBool32 Returns a boolean value indicating whether the Vulkan call that triggered the message should be aborted.
 */
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData)
{
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}


/**
 * @brief Creates a debug utils messenger for Vulkan instance.
 *
 * This function creates a debug utils messenger for the specified Vulkan instance.
 *
 * @param instance The Vulkan instance.
 * @param pCreateInfo Pointer to a structure containing the debug utils messenger creation information.
 * @param pAllocator Pointer to a structure specifying the allocation callbacks.
 * @param pDebugMessenger Pointer to a variable that will store the created debug utils messenger.
 * @return VkResult The result of the function call.
 *     - VK_SUCCESS: The debug utils messenger was successfully created.
 *     - VK_ERROR_EXTENSION_NOT_PRESENT: The required extension for debug utils messenger is not present.
 */
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}


/**
 * @brief Destroys a debug utils messenger extension.
 *
 * This function destroys a debug utils messenger extension associated with a Vulkan instance.
 *
 * @param instance The Vulkan instance.
 * @param debugMessenger The debug messenger to destroy.
 * @param pAllocator The allocation callbacks.
 */
void DestroyDebugUtilsMessengerEXT(VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks *pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}


/**
 * @brief Constructs a ZappyDevice object.
 *
 * This constructor initializes a ZappyDevice object with the provided ZappyWindow reference.
 * It performs various initialization steps such as creating an instance, setting up a debug messenger,
 * creating a surface, picking a physical device, creating a logical device, and creating a command pool.
 *
 * @param window The ZappyWindow reference to associate with the ZappyDevice object.
 */
ZappyDevice::ZappyDevice(ZappyWindow &window) : window{window}
{
    createInstance();
    setupDebugMessenger();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createCommandPool();
}


/**
 * @brief Destructor for the ZappyDevice class.
 * 
 * This destructor is responsible for cleaning up the resources used by the ZappyDevice object.
 * It destroys the Vulkan command pool, device, surface, and instance.
 * If validation layers are enabled, it also destroys the debug messenger.
 */
ZappyDevice::~ZappyDevice()
{
    vkDestroyCommandPool(device_, commandPool, nullptr);
    vkDestroyDevice(device_, nullptr);

    if (enableValidationLayers) {
        DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }

    vkDestroySurfaceKHR(instance, surface_, nullptr);
    vkDestroyInstance(instance, nullptr);
}


/**
 * @brief Creates a Vulkan instance for the ZappyDevice.
 *
 * This function creates a Vulkan instance for the ZappyDevice. It sets up the application information,
 * including the application name, version, engine name, engine version, and API version. It also enables
 * any required extensions and validation layers. If the creation of the instance fails, an exception is thrown.
 */
void ZappyDevice::createInstance() {
  if (enableValidationLayers && !checkValidationLayerSupport()) {
    throw zappy::UnavailableValidationLayersException();
  }

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "LittleVulkanEngine App";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount =
        static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    if (enableValidationLayers) {
        createInfo.enabledLayerCount =
            static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext =
            (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

  if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
    throw zappy::InstanceCreationFailedException();
  }

    hasGflwRequiredInstanceExtensions();
}


/**
 * Picks a suitable physical device for Vulkan rendering.
 * This function enumerates all available physical devices and selects the first one that meets the requirements.
 * If no suitable device is found, a GPUException is thrown.
 */
void ZappyDevice::pickPhysicalDevice() {
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
  if (deviceCount == 0) {
    throw zappy::GPUException();
  }
  std::cout << "Device count: " << deviceCount << std::endl;
  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto &device : devices) {
        if (isDeviceSuitable(device)) {
            physicalDevice = device;
            break;
        }
    }

  if (physicalDevice == VK_NULL_HANDLE) {
    throw zappy::GPUException();
  }

    vkGetPhysicalDeviceProperties(physicalDevice, &properties);
    std::cout << "physical device: " << properties.deviceName << std::endl;
}

/**
 * @brief Creates the logical device for the ZappyDevice.
 *
 * This function creates the logical device for the ZappyDevice by specifying the queue families,
 * device features, and enabled extensions. It also retrieves the graphics and present queues
 * from the created device.
 *
 * @throws zappy::LogicalDeviceCreationFailedException if the logical device creation fails.
 */
void ZappyDevice::createLogicalDevice()
{
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {
        indices.graphicsFamily, indices.presentFamily};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount =
        static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount =
        static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    // might not really be necessary anymore because device specific validation
    // layers have been deprecated
    if (enableValidationLayers) {
        createInfo.enabledLayerCount =
            static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

  if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device_) != VK_SUCCESS) {
    throw zappy::LogicalDeviceCreationFailedException();
  }

    vkGetDeviceQueue(device_, indices.graphicsFamily, 0, &graphicsQueue_);
    vkGetDeviceQueue(device_, indices.presentFamily, 0, &presentQueue_);
}

/**
 * @brief Creates a command pool for the ZappyDevice.
 *
 * This function creates a command pool for the ZappyDevice. The command pool is used to allocate command buffers.
 *
 * @throws zappy::CommandPoolCreationFailedException if the command pool creation fails.
 */
void ZappyDevice::createCommandPool()
{
    QueueFamilyIndices queueFamilyIndices = findPhysicalQueueFamilies();

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT |
        VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

  if (vkCreateCommandPool(device_, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
    throw zappy::CommandPoolCreationFailedException();
  }
}


/**
 * Creates the surface for the ZappyDevice.
 * This function creates a window surface using the provided instance and stores it in the surface_ member variable.
 */
void ZappyDevice::createSurface()
{
    window.createWindowSurface(instance, &surface_);
}

/**
 * @brief Checks if a physical device is suitable for use in the Zappy application.
 *
 * This function performs a series of checks to determine if the specified physical device
 * meets the requirements for running the Zappy application. It checks the device's queue
 * family indices, device extension support, swap chain support, and supported features.
 *
 * @param device The physical device to check.
 * @return True if the device is suitable, false otherwise.
 */
bool ZappyDevice::isDeviceSuitable(VkPhysicalDevice device)
{
    QueueFamilyIndices indices = findQueueFamilies(device);

    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport =
            querySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.formats.empty() &&
            !swapChainSupport.presentModes.empty();
    }

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

    return indices.isComplete() && extensionsSupported && swapChainAdequate &&
        supportedFeatures.samplerAnisotropy;
}


/**
 * @brief Populates the VkDebugUtilsMessengerCreateInfoEXT structure with the necessary information for creating a debug messenger.
 * 
 * @param createInfo The reference to the VkDebugUtilsMessengerCreateInfoEXT structure to be populated.
 */
void ZappyDevice::populateDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT &createInfo)
{
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr; // Optional
}


/**
 * @brief Sets up the debug messenger for the ZappyDevice.
 * 
 * This function is responsible for setting up the debug messenger for the ZappyDevice.
 * It checks if validation layers are enabled and creates a debug messenger using the Vulkan API.
 * If the creation of the debug messenger fails, it throws a DebugMessengerSetupFailedException.
 */
void ZappyDevice::setupDebugMessenger() {
  if (!enableValidationLayers) return;
  VkDebugUtilsMessengerCreateInfoEXT createInfo;
  populateDebugMessengerCreateInfo(createInfo);
  if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
    throw zappy::DebugMessengerSetupFailedException();
  }
}

/**
 * @brief Checks if the validation layers are supported by the Vulkan instance.
 * 
 * This function checks if the validation layers are supported by the Vulkan instance.
 * It enumerates the available instance layers and compares them with the required validation layers.
 * If any of the required validation layers are not found, it returns false.
 * 
 * @return True if the validation layers are supported, false otherwise.
 */
bool ZappyDevice::checkValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char *layerName : validationLayers) {
        bool layerFound = false;

        for (const auto &layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}


/**
 * @brief Retrieves the required extensions for the Vulkan instance.
 *
 * This function retrieves the required extensions for the Vulkan instance by calling `glfwGetRequiredInstanceExtensions`.
 * It also adds the `VK_EXT_DEBUG_UTILS_EXTENSION_NAME` extension if validation layers are enabled.
 *
 * @return A vector of const char* containing the required extensions.
 */
std::vector<const char *> ZappyDevice::getRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char *> extensions(
        glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}


/**
 * Checks if the required GLFW instance extensions are available.
 * 
 * This function enumerates the available Vulkan instance extensions and compares them
 * with the required extensions for GLFW. If any of the required extensions are missing,
 * it throws a `zappy::MissingGLFWExtensionException`.
 */
void ZappyDevice::hasGflwRequiredInstanceExtensions()
{
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(
        nullptr, &extensionCount, extensions.data());

    std::cout << "available extensions:" << std::endl;
    std::unordered_set<std::string> available;
    for (const auto &extension : extensions) {
        std::cout << "\t" << extension.extensionName << std::endl;
        available.insert(extension.extensionName);
    }

  std::cout << "required extensions:" << std::endl;
  auto requiredExtensions = getRequiredExtensions();
  for (const auto &required : requiredExtensions) {
    std::cout << "\t" << required << std::endl;
    if (available.find(required) == available.end()) {
      throw zappy::MissingGLFWExtensionException();
    }
  }
}

/**
 * @brief Checks if a physical device supports the required device extensions.
 * 
 * This function checks if a physical device supports the required device extensions.
 * It enumerates the available device extensions and compares them with the required extensions.
 * If any of the required extensions are missing, it returns false.
 * 
 * @param device The physical device to check.
 * @return True if the device supports the required extensions, false otherwise.
 */
bool ZappyDevice::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(
        device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(
        device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(
        deviceExtensions.begin(), deviceExtensions.end());

    for (const auto &extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}


/**
 * Finds the queue families supported by the specified physical device.
 *
 * @param device The Vulkan physical device.
 * @return The queue family indices containing the graphics and present families.
 */
QueueFamilyIndices ZappyDevice::findQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(
        device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(
        device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto &queueFamily : queueFamilies) {
        if (queueFamily.queueCount > 0 &&
            queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
            indices.graphicsFamilyHasValue = true;
        }
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(
            device, i, surface_, &presentSupport);
        if (queueFamily.queueCount > 0 && presentSupport) {
            indices.presentFamily = i;
            indices.presentFamilyHasValue = true;
        }
        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}


/**
 * Queries the swap chain support details for a given physical device.
 *
 * @param device The physical device to query.
 * @return The swap chain support details.
 */
SwapChainSupportDetails ZappyDevice::querySwapChainSupport(
    VkPhysicalDevice device)
{
    SwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        device, surface_, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(
        device, surface_, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(
            device, surface_, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device, surface_, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(
            device, surface_, &presentModeCount, details.presentModes.data());
    }
    return details;
}


/**
 * Finds a supported Vulkan format from a list of candidates.
 *
 * @param candidates The list of Vulkan formats to check for support.
 * @param tiling The tiling mode of the image.
 * @param features The required format features.
 * @return The supported Vulkan format.
 * @throws zappy::NoSupportedFormatException if no supported format is found.
 */
VkFormat ZappyDevice::findSupportedFormat(
    const std::vector<VkFormat> &candidates, VkImageTiling tiling,
    VkFormatFeatureFlags features)
{
    for (VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

    if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
      return format;
    } else if (
        tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
      return format;
    }
  }
  throw zappy::NoSupportedFormatException();
}

/**
 * Finds a suitable memory type for the specified type filter and properties.
 *
 * @param typeFilter The type filter to use.
 * @param properties The memory properties to use.
 * @return The suitable memory type index.
 * @throws zappy::NoSuitableMemoryTypeException if no suitable memory type is found.
 */
uint32_t ZappyDevice::findMemoryType(
    uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) &&
            (memProperties.memoryTypes[i].propertyFlags & properties) ==
                properties) {
            return i;
        }
    }

  throw zappy::NoSuitableMemoryTypeException();
}

/**
 * @brief Creates a buffer for the ZappyDevice.
 *
 * This function creates a buffer for the ZappyDevice with the specified size, usage, and properties.
 * It creates a buffer and allocates memory for it using the Vulkan API.
 *
 * @param size The size of the buffer.
 * @param usage The usage flags for the buffer.
 * @param properties The memory properties for the buffer.
 * @param buffer The reference to the buffer to create.
 * @param bufferMemory The reference to the buffer memory to allocate.
 * @throws zappy::VertexBufferCreationFailedException if the buffer creation fails.
 * @throws zappy::MemoryAllocationFailedException if the memory allocation fails.
 */
void ZappyDevice::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties, VkBuffer &buffer,
    VkDeviceMemory &bufferMemory)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateBuffer(device_, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
    throw zappy::VertexBufferCreationFailedException();
  }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device_, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex =
        findMemoryType(memRequirements.memoryTypeBits, properties);

  if (vkAllocateMemory(device_, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
    throw zappy::MemoryAllocationFailedException();
  }

    vkBindBufferMemory(device_, buffer, bufferMemory, 0);
}

/**
 * @brief Begins a single time command for the ZappyDevice.
 *
 * This function begins a single time command for the ZappyDevice. It allocates a command buffer
 * and begins recording commands to it.
 *
 * @return The command buffer that was allocated.
 */
VkCommandBuffer ZappyDevice::beginSingleTimeCommands()
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device_, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);
    return commandBuffer;
}


/**
 * @brief Ends a single time command buffer and submits it to the graphics queue.
 *
 * This function ends the specified command buffer and submits it to the graphics queue for execution.
 * It also waits for the graphics queue to become idle before freeing the command buffer.
 *
 * @param commandBuffer The command buffer to end and submit.
 */
void ZappyDevice::endSingleTimeCommands(VkCommandBuffer commandBuffer)
{
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(graphicsQueue_, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue_);

    vkFreeCommandBuffers(device_, commandPool, 1, &commandBuffer);
}


/**
 * Copies data from one Vulkan buffer to another.
 *
 * @param srcBuffer The source buffer to copy from.
 * @param dstBuffer The destination buffer to copy to.
 * @param size The size of the data to copy.
 */
void ZappyDevice::copyBuffer(
    VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0; // Optional
    copyRegion.dstOffset = 0; // Optional
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    endSingleTimeCommands(commandBuffer);
}


/**
 * Copies the contents of a buffer to an image.
 *
 * @param buffer The source buffer to copy from.
 * @param image The destination image to copy to.
 * @param width The width of the image.
 * @param height The height of the image.
 * @param layerCount The number of layers in the image.
 */
void ZappyDevice::copyBufferToImage(VkBuffer buffer, VkImage image,
    uint32_t width, uint32_t height, uint32_t layerCount)
{
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = layerCount;

    region.imageOffset = {0, 0, 0};
    region.imageExtent = {width, height, 1};

    vkCmdCopyBufferToImage(commandBuffer, buffer, image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
    endSingleTimeCommands(commandBuffer);
}


/**
 * Creates a Vulkan image with the specified image information and memory properties.
 * 
 * @param imageInfo The Vulkan image creation information.
 * @param properties The memory property flags for the image.
 * @param image The created Vulkan image.
 * @param imageMemory The device memory associated with the image.
 * 
 * @throws zappy::ImageCreationFailedException if the image creation fails.
 * @throws zappy::MemoryAllocationFailedException if the memory allocation fails.
 * @throws zappy::ImageMemoryBindFailedException if the image memory binding fails.
 */
void ZappyDevice::createImageWithInfo(
    const VkImageCreateInfo &imageInfo,
    VkMemoryPropertyFlags properties,
    VkImage &image,
    VkDeviceMemory &imageMemory) {
  if (vkCreateImage(device_, &imageInfo, nullptr, &image) != VK_SUCCESS) {
    throw zappy::ImageCreationFailedException();
  }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device_, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex =
        findMemoryType(memRequirements.memoryTypeBits, properties);

  if (vkAllocateMemory(device_, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
    throw zappy::MemoryAllocationFailedException();
  }

  if (vkBindImageMemory(device_, image, imageMemory, 0) != VK_SUCCESS) {
    throw zappy::ImageMemoryBindFailedException();
  }
}

} // namespace zappy
