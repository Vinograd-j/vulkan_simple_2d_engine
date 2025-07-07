#pragma once
#include "allocator.h"
#include "../../pipeline/include/graphics-pipeline.h"
#include "../../swapchain/include/present-swapchain.h"
#include "../command-buffer/include/command-buffers.h"
#include "../../struct/vertex.h"
#include "../command-buffer/include/command-pool.h"

class Renderer
{

private:

    CommandBuffers _commandBuffers;

    const GraphicsPipeline* const _pipeline;

    PresentSwapchain* _swapchain;

    std::vector<VkFence> _inFlightFences;
    std::vector<VkSemaphore> _renderingFinishedSemaphores;
    std::vector<VkSemaphore> _imageAvailableSemaphores;

    std::vector<VkImageView> _imageViews;

    const LogicalDevice* const _device;

    const int MAX_FRAMES_IN_FLIGHT = 2;
    uint32_t _currentFrame = 0;

    const Allocator* const _allocator;

    VkBuffer _vertexBuffer;
    VmaAllocation _vertexBufferMemory;

    VkBuffer _indexBuffer;
    VmaAllocation _indexBufferMemory;

    const std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
    };

    const std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0
    };

    VkQueue _graphicsQueue;
    VkQueue _presentQueue;

    std::vector<VkImageLayout> _swapchainImageLayouts;

    const CommandPool* const _pool;

public:

    explicit Renderer(const CommandPool* pool, const CommandBuffers& command_buffers, const GraphicsPipeline* pipeline, PresentSwapchain* swapchain, const LogicalDevice* device, VkQueue graphics, VkQueue present, const Allocator* allocator);

    void DrawFrame();

    ~Renderer();

private:

    void CreateSyncObjects();

    void RecordCommandBuffer(VkCommandBuffer buffer, VkImageView imageView, uint32_t imageIndex);

    void CreateVertexBuffer();
    void CreateIndexBuffer();

    VkImageSubresourceRange GetImageSubresourceRange() const;

    VkComponentMapping GetComponentMapping() const;

    void RecreateSwapchain();

    void CopyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size);

};
