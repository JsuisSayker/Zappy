/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Texture
*/

#pragma once

#include "Device.hpp"
#include <string.h>
#include <vulkan/vulkan_core.h>

namespace zappy {
class Texture {
  public:
    Texture(ZappyDevice &device, const std::string &filepath);
    ~Texture();

    Texture(const Texture &) = delete;
    Texture &operator=(const Texture &) = delete;
    Texture(Texture &&) = delete;
    Texture &operator=(Texture &&) = delete;

    VkSampler getSampler() { return sampler; }
    VkImageView getImageView() { return imageView; }
    VkImageLayout getImageLayout() { return imageLayout; }

  private:
    void transitionImageLayout(
        VkImageLayout oldLayout, VkImageLayout newLayout);
    void generateMipmaps();

    int width, height, mipLevels;

    ZappyDevice &lveDevice;
    VkImage image;
    VkDeviceMemory imageMemory;
    VkImageView imageView;
    VkSampler sampler;
    VkFormat imageFormat;
    VkImageLayout imageLayout;
};
} // namespace zappy
