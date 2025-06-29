#pragma once

#include <vector>

#include "logical-device.h"
#include "vulkan/vulkan.h"

class CommandBuffers
{

private:

    std::vector<VkCommandBuffer> _commandBuffers;

    VkCommandPool _pool;

    const LogicalDevice* _device;

public:

    explicit CommandBuffers(uint32_t count, const VkCommandBufferLevel& level, VkCommandPool pool, const LogicalDevice* device);

    std::vector<VkCommandBuffer> GetCommandBuffer() const { return _commandBuffers; }

    VkCommandBuffer& operator[](const int index) { return _commandBuffers[index]; }

private:

    void CreateCommandBuffers(const VkCommandBufferLevel& level, uint32_t count);

};
