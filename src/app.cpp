#include "app.hpp"

App::App()
{
	window = nullptr;
	color[0] = {1.f,0.f,0.f};
	color[1] = {0.f,1.f,0.f};
	color[2] = {0.f,0.f,1.f};
}

void App::run()
{
	initWindow();

	createInstance();
	createSurface();
	vulkan.initVulkan(&instance, &surface);
	helloTriangle.initilaze(vulkan);
	mainLoop();
	cleanup();
}

void App::initWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	glfwSetKeyCallback(window, (&key_callback));
}

void App::mainLoop()
{
	while (!glfwWindowShouldClose(window))
	{
		glfwWaitEvents();
		processColor();
		helloTriangle.createCommandBuffers();
		vulkan.drawFrame();
	}

	vkDeviceWaitIdle(vulkan.getDevice());
}

void App::cleanup()
{

	helloTriangle.cleanup();
	vulkan.shutdown();

	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window);

	glfwTerminate();
}


void App::fillAppInfo(VkApplicationInfo& appInfo)
{
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;
}

void App::createInstance()
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

void App::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
}


void App::createSurface()
{
	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create window surface!");
	}
}

std::vector<const char*> App::getRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	return extensions;
}

void App::processColor()
{
	for (int i = 0; i < 3; i++)
	{
		if (color[i].x > 0 && color[i].z <= 0)
		{
			color[i].x -= 0.01f;
			color[i].y += 0.01f;
			color[i].z = 0.f;
		}
		else if (color[i].y > 0 && color[i].x <= 0)
		{
			color[i].x = 0.f;
			color[i].y -= 0.01f;
			color[i].z += 0.01f;
		}
		else
		{
			color[i].x += 0.01f;
			color[i].y = 0.f;
			color[i].z -= 0.01f;
		}
	}
	helloTriangle.updateColor(color[0], color[1], color[2]);
	return;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	return;
}