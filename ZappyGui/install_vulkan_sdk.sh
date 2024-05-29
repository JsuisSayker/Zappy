#!/bin/bash

# Install Vulkan SDK
if [ ! -f "./vulkan-sdk.tar.gz" ]; then
    curl https://sdk.lunarg.com/sdk/download/latest/linux/vulkan-sdk.tar.gz --output vulkan-sdk.tar.gz
fi
if [ ! -d "./vulkan-sdk" ]; then
    mkdir -p vulkan-sdk
    tar xf vulkan-sdk.tar.gz -C vulkan-sdk
fi
cd "vulkan-sdk/" || exit 1
if [ ! -d "./sdk" ]; then
    mv * sdk
fi
cd sdk || exit 1
chmod +x setup-env.sh
./setup-env.sh
./vulkansdk

# # Set environment variables
# export VULKAN_SDK=$PWD/vulkan-sdk/sdk/x86_64
# export PATH=$VULKAN_SDK/bin:$PATH
# export LD_LIBRARY_PATH=$VULKAN_SDK/lib:$LD_LIBRARY_PATH
# export VK_LAYER_PATH=$VULKAN_SDK/etc/vulkan/explicit_layer.d
