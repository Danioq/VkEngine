#pragma once


#include "VkCnfg.hpp"
#include "HelloTriangleApplication.hpp"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

namespace
{
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
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
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			func(instance, debugMessenger, pAllocator);
		}
	}

}


class App
{
public:
	App();
	void run();
	void cleanup();
private:
	GLFWwindow* window;

	VkInstance instance;
	VkSurfaceKHR surface;

	Vulkan vulkan;
	glm::vec3 color[3];
	HelloTriangleApplication helloTriangle;
	void processColor();
	void initWindow();

	void mainLoop();

	void createInstance();

	void fillAppInfo(VkApplicationInfo&);

	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT&);
	void createSurface();

	std::vector<const char*> getRequiredExtensions();

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}
};