#pragma once

#include "allocator.h"
#include "../../command-buffer/include/command-pool.h"

template<typename T>
class Buffer
{

private:

    VkBuffer _buffer {};
    VmaAllocation _bufferMemory {};

    const Allocator* const _allocator;

    const CommandPool* _pool;

    const LogicalDevice* _device;

public:

    explicit Buffer(const Allocator* allocator, const CommandPool* pool, const LogicalDevice* device)  :
                                                                                                        _allocator(allocator),
                                                                                                        _pool(pool),
                                                                                                        _device(device){}

    void CreateBuffer(const VkBufferUsageFlags& usage, const VmaMemoryUsage& memoryUsage, std::vector<T> data);
    void CreateBufferWithStaging(const VkBufferUsageFlags& usage, const VmaMemoryUsage& memoryUsage, std::vector<T> data);

    VkBuffer GetBuffer() const { return _buffer; }

    ~Buffer();

private:

    void CopyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size) const;

};

template<typename T>
void Buffer<T>::CreateBuffer(const VkBufferUsageFlags& usage, const VmaMemoryUsage& memoryUsage, std::vector<T> data)
{
    VkDeviceSize bufferSize = sizeof(data[0]) * data.size();

    VkBufferCreateInfo vertexBufferInfo{};
    vertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vertexBufferInfo.size = bufferSize;
    vertexBufferInfo.usage = usage;
    vertexBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo allocCreateInfo{};
    allocCreateInfo.usage = memoryUsage;
    allocCreateInfo.flags = 0;

    VmaAllocationInfo allocInfo;
    _allocator->CreateBuffer(vertexBufferInfo, allocCreateInfo, &_buffer, &_bufferMemory, &allocInfo);

    memcpy(allocInfo.pMappedData, data.data(), bufferSize);
}

template<typename T>
void Buffer<T>::CreateBufferWithStaging(const VkBufferUsageFlags& usage, const VmaMemoryUsage& memoryUsage, std::vector<T> data)
{
    VkDeviceSize bufferSize = sizeof(data[0]) * data.size();

    VkBuffer stagingBuffer;
    VmaAllocation stagingMemory;
    VmaAllocationInfo stagingAllocInfo;

    VkBufferCreateInfo stagingCreateInfo {};
    stagingCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    stagingCreateInfo.size = bufferSize;
    stagingCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    stagingCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo stagingAllocCreateInfo{};
    stagingAllocCreateInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;
    stagingAllocCreateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;

    _allocator->CreateBuffer(stagingCreateInfo, stagingAllocCreateInfo, &stagingBuffer, &stagingMemory, &stagingAllocInfo);

    memcpy(stagingAllocInfo.pMappedData, data.data(), bufferSize);

    VkBufferCreateInfo vertexBufferInfo{};
    vertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vertexBufferInfo.size = bufferSize;
    vertexBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage;
    vertexBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo allocInfo{};
    allocInfo.usage = memoryUsage;
    allocInfo.flags = 0;

    _allocator->CreateBuffer(vertexBufferInfo, allocInfo, &_buffer, &_bufferMemory, nullptr);

    CopyBuffer(stagingBuffer, _buffer, bufferSize);
    vmaDestroyBuffer(_allocator->GetAllocator(), stagingBuffer, stagingMemory);
}

template<typename T>
void Buffer<T>::CopyBuffer(const VkBuffer src, const VkBuffer dst, VkDeviceSize size) const
{
    VkCommandBufferAllocateInfo allocateInfo {};
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocateInfo.commandPool = _pool->GetCommandPool();
    allocateInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(_device->GetDevice(), &allocateInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkBufferCopy copyRegion {};
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, src, dst, 1, &copyRegion);

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(_device->GetQueues().at(GRAPHICS), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(_device->GetQueues().at(GRAPHICS));

    vkFreeCommandBuffers(_device->GetDevice(), _pool->GetCommandPool(), 1, &commandBuffer);
}

template<typename T>
Buffer<T>::~Buffer()
{
    vmaDestroyBuffer(_allocator->GetAllocator(), _buffer, _bufferMemory);
}