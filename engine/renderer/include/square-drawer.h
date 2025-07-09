#pragma once

#include "allocator.h"
#include "image-memory-barrier.h"
#include "synchronization.h"
#include "../../pipeline/include/graphics-pipeline.h"
#include "../../swapchain/include/present-swapchain.h"
#include "../command-buffer/include/command-buffers.h"
#include "../command-buffer/include/command-pool.h"
#include "../buffers/include/vertex-buffer.h"
#include "../buffers/include/index-buffer.h"

class SquareDrawer
{

private:

    const LogicalDevice* const _device;

    PresentSwapchain* _swapchain;

    const GraphicsPipeline* const _pipeline;

    const Allocator* const _allocator;

    const CommandPool* const _pool;

private:

    CommandBuffers _commandBuffers;

    Synchronization _syncObjects;

    std::vector<VkImageView> _imageViews;

    std::unique_ptr<VertexBuffer> _vertexBuffer;
    std::unique_ptr<IndexBuffer> _indexBuffer;

    std::vector<VkImageLayout> _swapchainImageLayouts;

    ImageMemoryBarrier barrier;

private:

    uint32_t _currentFrame = 0;

public:

    explicit SquareDrawer(const CommandPool* pool, const CommandBuffers& command_buffers, const GraphicsPipeline* pipeline, PresentSwapchain* swapchain, const LogicalDevice* device, const Allocator* allocator);

    void DrawFrame();

    ~SquareDrawer();

private:

    void RecordCommandBuffer(VkCommandBuffer buffer, VkImageView imageView, uint32_t imageIndex);

    void CreateVertexBuffer();
    void CreateIndexBuffer();

    VkImageSubresourceRange GetImageSubresourceRange() const;

    VkComponentMapping GetComponentMapping() const;

    void RecreateSwapchain();

};