#include "../include/allocator.h"

#include <stdexcept>

Allocator::Allocator(const VkPhysicalDevice& device, const VkInstance& instance) : _device(device), _instance(instance)
{
    CreateAllocator();
}

void Allocator::CreateBuffer(const VkBufferCreateInfo& createInfo, VmaAllocationCreateInfo& allocationInfo, VkBuffer* buffer, VmaAllocation* memory) const
{
    vmaCreateBuffer(_allocator, &createInfo, &allocationInfo, buffer, memory, nullptr);
}

void Allocator::CreateImage(const VkImageCreateInfo& createInfo, VmaAllocationCreateInfo& allocationInfo, VkImage* image, VmaAllocation* memory) const
{
    vmaCreateImage(_allocator, &createInfo, &allocationInfo, image, memory, nullptr);
}

void Allocator::CreateAllocator()
{
    VmaVulkanFunctions vulkanFunctions {};
    vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
    vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

    VmaAllocatorCreateInfo createInfo {};
    createInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
    createInfo.vulkanApiVersion = VK_API_VERSION_1_3;
    createInfo.physicalDevice = _device;
    createInfo.instance = _instance;
    createInfo.pVulkanFunctions = &vulkanFunctions;

    if (vmaCreateAllocator(&createInfo, &_allocator) != VK_SUCCESS)
        throw std::runtime_error("failed to create vulkan memory allocator (vma)");
}

Allocator::~Allocator()
{
    vmaDestroyAllocator(_allocator);
}