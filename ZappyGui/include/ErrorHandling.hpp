/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ErrorHandling
*/

#pragma once

#include <exception>

namespace zappy {
    class DescriptorSetLayoutCreationFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to create descriptor set layout.");
            }
    };
    class DescriptorPoolCreationFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to create descriptor pool.");
            }
    };
    class UnavailableValidationLayersException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Validation layers requested, but not available.");
            }
    };
    class InstanceCreationFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to create instance.");
            }
    };
    class GPUException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to find GPUs that support Vulkan/ are suitable.");
            }
    };
    class LogicalDeviceCreationFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to create logical device.");
            }
    };
    class CommandPoolCreationFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to create command pool.");
            }
    };
    class DebugMessengerSetupFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to set up debug messenger.");
            }
    };
    class MissingGLFWExtensionException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Required glfw extension is missing.");
            }
    };
    class NoSupportedFormatException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to find supported format.");
            }
    };
    class NoSuitableMemoryTypeException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to find suitable memory type.");
            }
    };
    class VertexBufferCreationFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to create vertex buffer.");
            }
    };
    class MemoryAllocationFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to allocate memory.");
            }
    };
    class ImageCreationFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to create image.");
            }
    };
    class ImageMemoryBindFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to bind image memory.");
            }
    };
    class OpenFileFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to open file.");
            }
    };
    class GraphicsPipelineCreationFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to create graphics pipeline.");
            }
    };
    class ShaderModuleCreationFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to create shader module.");
            }
    };
    class PipelineLayoutCreationFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to create pipeline layout.");
            }
    };
    class SwapChainFormatChangedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Swap chain image or depth format has changed.");
            }
    };
    class SwapChainImageAdquisitionFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to acquire swap chain image.");
            }
    };
    class SwapChainImagePresentationFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to present swap chain image.");
            }
    };
    class LoadObjFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("LoadObj function returned false.");
            }
    };
    class CommandBufferRecordingFailedToBeginException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to begin recording command buffer.");
            }
    };
    class CommandBufferRecordingFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to record command buffer.");
            }
    };
    class DrawCommandBufferSubmitFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to submit draw command buffer.");
            }
    };
    class SwapChainCreationFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to create swap chain.");
            }
    };
    class TextureImageViewCreationFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to create texture image view.");
            }
    };
    class RenderPassCreationFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to create render pass.");
            }
    };
    class FrameBufferCreationFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to create frame buffer.");
            }
    };
    class FrameSyncObjectsCreationFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to create synchronization objects for a frame.");
            }
    };
    class LayoutTransitionUnsupportedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Unsupported layout transition.");
            }
    };
    class LinearBlittingNotSupportedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Texture image format does not support linear blitting.");
            }
    };
    class WindowSurfaceCreationFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to create window surface.");
            }
    };
    class ReadLinkFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to read the symbolic link.");
            }
    };

    class TratorianNotFoundException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Trantorian not found.");
            }
    };
}
