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
