#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <optional>
#include <set>
#include <glm/glm.hpp>
#include <array>

const int WIDTH = 800;
const int HEIGHT = 600;

const int MAX_FRAMES_IN_FLIGHT = 2;
namespace 
{
const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };


VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, 
                                      const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
                                      const VkAllocationCallbacks* pAllocator, 
                                      VkDebugUtilsMessengerEXT* pDebugMessenger) 
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) 
    {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else 
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                   VkDebugUtilsMessengerEXT debugMessenger,
                                   const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        func(instance, debugMessenger, pAllocator);
    }
}

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete()
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};
}


class HelloTriangleApplication
{
public:
    void run();

private:
    GLFWwindow* window;

    VkInstance instance;
    VkSurfaceKHR surface;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkRenderPass renderPass;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    size_t currentFrame = 0;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

    typedef struct Vertex
    {
        glm::vec2 pos;
        glm::vec3 color;

        static VkVertexInputBindingDescription getBindingDestription()
        {
            VkVertexInputBindingDescription bindingDestription = {};
            bindingDestription.binding =0;
            bindingDestription.stride = sizeof(Vertex);
            bindingDestription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDestription;
        }

        static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
        {
            std::array<VkVertexInputAttributeDescription, 2> attributeDesctription = {};
            attributeDesctription[0].binding = 0;
            attributeDesctription[0].location = 0;
            attributeDesctription[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDesctription[0].offset = offsetof(Vertex, pos);

            attributeDesctription[1].binding = 0;
            attributeDesctription[1].location = 1;
            attributeDesctription[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDesctription[1].offset = offsetof(Vertex, color);

            return attributeDesctription;
        }
    } Vertex;

    std::vector<Vertex> vertices = 
    {
        {{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };

    void initWindow();
    void initVulkan();

    void mainLoop();
    void cleanup();
    void createInstance();

    void fillAppInfo(VkApplicationInfo&);

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT&);
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createSwapChain();
    void createImageViews();
    void createRenderPass();
    void createGraphicsPipeline();
    void createFramebuffers();
    void createCommandPool();
    void createCommandBuffers();
    void createSyncObjects();
    void drawFrame();
    void createVertexBuffer();
    uint32_t findMemoryType(uint16_t, VkMemoryPropertyFlags);
    VkShaderModule createShaderModule(const std::vector<char>&);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>&);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>&);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR&);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice);
    bool isDeviceSuitable(VkPhysicalDevice);
    bool checkDeviceExtensionSupport(VkPhysicalDevice);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice);
    std::vector<const char*> getRequiredExtensions();

    static std::vector<char> readFile(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);
        if (!file.is_open())
        {
            throw std::runtime_error("failed to open file!");
        }
        size_t fileSize = (size_t) file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return buffer;
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
                                                        VkDebugUtilsMessageTypeFlagsEXT messageType, 
                                                        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
                                                        void* pUserData)
    {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
    }
};