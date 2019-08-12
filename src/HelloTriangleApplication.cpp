#include "HelloTriangleApplication.hpp"

HelloTriangleApplication::HelloTriangleApplication()
{
	this->vulkan = nullptr;
}

void HelloTriangleApplication::initilaze(Vulkan &vulkan)
{
	this->vulkan = &vulkan;
	createVertexBuffer();
}

void HelloTriangleApplication::cleanup()
{
    vkDestroyBuffer(vulkan->getDevice(), vertexBuffer, nullptr);
    vkFreeMemory(vulkan->getDevice(), vertexBufferMemory, nullptr);
	vulkan = nullptr;
	return;
}

void HelloTriangleApplication::createVertexBuffer()
{
	vertices = {{{ 0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
				{{ 0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}},
				{{-0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}}};
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = sizeof(vertices[0]) * vertices.size();
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	VkDevice device = vulkan->getDevice();

    if(vkCreateBuffer(device, &bufferInfo, nullptr, &vertexBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create vertex buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, vertexBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = vulkan->findMemoryType(memRequirements.memoryTypeBits,
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

void HelloTriangleApplication::render()
{
	vulkan->render(vertexBuffer, vertices);
	return;
}

void HelloTriangleApplication::updateColor(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{	
	void* data;
	unsigned long long size = sizeof(vertices[0]) * vertices.size();
	VkDevice device = vulkan->getDevice();
	vertices[0].color = v1;
	vertices[1].color = v2;
	vertices[2].color = v3;
	
	vkMapMemory(device, vertexBufferMemory, 0, size, 0, &data);
	memcpy(data, vertices.data(), (size_t(size)));
	vkUnmapMemory(device, vertexBufferMemory);
}