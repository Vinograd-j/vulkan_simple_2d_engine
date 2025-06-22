#pragma once

#include "vk_mem_alloc.h"

class Allocator
{

private:

    VmaAllocator _allocator {};

private:

    VkPhysicalDevice _device;

    VkInstance _instance;

public:

    explicit Allocator(const VkPhysicalDevice& device, const VkInstance& instance);

    ~Allocator();

public:

    void CreateBuffer(const VkBufferCreateInfo& createInfo, VmaAllocationCreateInfo& allocationInfo, VkBuffer* buffer, VmaAllocation* memory) const;

    void CreateImage(const VkImageCreateInfo& createInfo, VmaAllocationCreateInfo& allocationInfo, VkImage* image, VmaAllocation* memory) const;

private:

    void CreateAllocator();

};