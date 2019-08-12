#pragma once

#include "VkCnfg.hpp"

class HelloTriangleApplication
{
public:
	HelloTriangleApplication();
	void initilaze(Vulkan&);
    void cleanup();
	void render();
	void updateColor(glm::vec3, glm::vec3, glm::vec3);
private:
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

	Vulkan *vulkan;

    std::vector<Vulkan::Vertex> vertices;

    void createVertexBuffer();
	
};
