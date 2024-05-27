/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** error_handling
*/

#pragma once

#include <exception>

namespace zappy {
    class DSLCreationFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to create descriptor set layout.");
            }
    };
    class DPCreationFailedException : public std::exception {
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
    class LDCreationFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to create logical device.");
            }
    };
    class CPCreationFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to create command pool.");
            }
    };
    class DBSetupFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to set up debug messenger.");
            }
    };
    class GLFWExtensionException : public std::exception {
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
    class VBCreationFailedException : public std::exception {
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
    class GPCreationFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to create graphics pipeline.");
            }
    };
    class SMCreationFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to create shader module.");
            }
    };
    class PLCreationFailedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Failed to create pipeline layout.");
            }
    };
    class SCFormatChangedException : public std::exception {
        public:
            const char *what() const noexcept override
            {
                return ("Swap chain image or depth format has changed.");
            }
    };
}
