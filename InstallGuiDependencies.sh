#!/bin/bash

# Detect Linux distribution
if [ -f /etc/os-release ]; then
    . /etc/os-release
    OS=$ID
else
    echo "Cannot detect the operating system."
    exit 1
fi

# Install Vulkan components, GLFW, GLM, and glslc based on the distribution
case $OS in
    ubuntu|debian)
        echo "Installing Vulkan components, GLFW, GLM, glslc, and dependencies on Debian/Ubuntu..."
        sudo apt update
        sudo apt install -y vulkan-tools libvulkan-dev vulkan-validationlayers-dev spirv-tools libglfw3-dev libglm-dev libxxf86vm-dev libxi-dev

        # Download and install glslc
        GLSLC_URL="https://github.com/google/shaderc/releases/download/v2022.2/shaderc-linux-x64-release.zip"
        wget $GLSLC_URL -O shaderc.zip
        unzip shaderc.zip -d shaderc
        sudo cp shaderc/bin/glslc /usr/local/bin/
        rm -rf shaderc.zip shaderc
        ;;
    fedora)
        echo "Installing Vulkan components, GLFW, GLM, glslc, and dependencies on Fedora..."
        sudo dnf install -y glslc glfw-devel vulkan-devel glm-devel gtest-devel
        ;;
    arch)
        echo "Installing Vulkan components, GLFW, GLM, glslc, and dependencies on Arch Linux..."
        sudo pacman -Syu --needed vulkan-devel glfw-wayland glm libxi libxxf86vm shaderc
        # Uncomment the next line for X11 users
        # sudo pacman -Syu --needed vulkan-devel glfw-x11 glm libxi libxxf86vm shaderc
        ;;
    *)
        echo "Unsupported Linux distribution: $OS"
        exit 1
        ;;
esac

if [ ! -d "ZappyGui/vulkan-sdk" ]; then
    cd ZappyGui || exit 1
    ./install_vulkan_sdk.sh
else
    echo "Vulkan SDK is already installed."
fi

echo "------------------------------------------------------------"
echo "Now you can run the following commands to build the project:"
echo 'export VULKAN_SDK="${PWD}/ZappyGui/vulkan-sdk/sdk/x86_64"'
echo 'export PATH="$VULKAN_SDK/bin:$PATH"'
echo 'export LD_LIBRARY_PATH="$VULKAN_SDK/lib:$LD_LIBRARY_PATH"'
echo 'export VK_LAYER_PATH="$VULKAN_SDK/share/vulkan/explicit_layer.d"'