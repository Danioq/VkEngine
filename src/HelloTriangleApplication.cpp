#include "HelloTriangleApplication.hpp"

HelloTriangleApplication::HelloTriangleApplication()
{
	window = nullptr;
}

void HelloTriangleApplication::run()
{
    initWindow();

	createInstance();
	createSurface();
    vulkan.initVulkan(&instance, &surface);

	createVertexBuffer();

    mainLoop();
    cleanup();
}

void HelloTriangleApplication::initWindow()
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	glfwSetKeyCallback(window, (&key_callback));
}

void HelloTriangleApplication::mainLoop()
{
    while (!glfwWindowShouldClose(window))
    {
        glfwWaitEvents();

		vulkan.createCommandBuffers(vertexBuffer, vertices);
		vulkan.drawFrame();
    }

    vkDeviceWaitIdle(vulkan.getDevice());
}

void HelloTriangleApplication::cleanup()
{
    vkDestroyBuffer(vulkan.getDevice(), vertexBuffer, nullptr);
    vkFreeMemory(vulkan.getDevice(), vertexBufferMemory, nullptr);
	
	vulkan.shutdown();

    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(window);

    glfwTerminate();
}


void HelloTriangleApplication::fillAppInfo(VkApplicationInfo& appInfo)
{
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
}

void HelloTriangleApplication::createInstance()
{
    VkApplicationInfo appInfo = {};
    fillAppInfo(appInfo);

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    createInfo.enabledLayerCount = 0;
    createInfo.pNext = nullptr;

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create instance!");
    }
}

void HelloTriangleApplication::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | 
                                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT    | 
                                VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | 
                                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}


void HelloTriangleApplication::createSurface()
{
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create window surface!");
    }
}

std::vector<const char*> HelloTriangleApplication::getRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    return extensions;
}

void HelloTriangleApplication::createVertexBuffer()
{
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = sizeof(vertices[0]) * vertices.size();
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	VkDevice device = vulkan.getDevice();

    if(vkCreateBuffer(device, &bufferInfo, nullptr, &vertexBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create vertex buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, vertexBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = vulkan.findMemoryType(memRequirements.memoryTypeBits,
                                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                               VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if(vkAllocateMemory(device, &allocInfo, nullptr, &vertexBufferMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate vertex buffer memory");
    }

    vkBindBufferMemory(device, vertexBuffer, vertexBufferMemory, 0);
    void* data;
    vkMapMemory(device, vertexBufferMemory, 0, bufferInfo.size, 0, &data);
    memcpy(data, vertices.data(), (size_t(bufferInfo.size)));
    vkUnmapMemory(device, vertexBufferMemory);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) 
	{ 	
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	return;
}

