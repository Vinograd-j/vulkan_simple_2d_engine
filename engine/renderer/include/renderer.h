#pragma once
#include "allocator.h"
#include "../../pipeline/include/graphics-pipeline.h"
#include "../../swapchain/include/present-swapchain.h"
#include "../command-buffer/include/command-buffers.h"
#include "../../struct/vertex.h"

class Renderer
{

private:

    CommandBuffers _commandBuffers;

    const GraphicsPipeline* _pipeline;

    const PresentSwapchain* _swapchain;

    std::vector<VkFence> _inFlightFences;
    std::vector<VkSemaphore> _renderingFinishedSemaphores;
    std::vector<VkSemaphore> _imageAvailableSemaphores;

    std::vector<VkImageView> _imageViews;

    const LogicalDevice* _device;

    const int MAX_FRAMES_IN_FLIGHT = 2;
    uint32_t _currentFrame = 0;

    //Allocator _allocator;

    VkBuffer _vertexBuffer;
    VkBuffer _indexBuffer;

    const std::vector<Vertex> vertices = {
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };

    const std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0
    };

    VkQueue _graphicsQueue;
    VkQueue _presentQueue;

    std::vector<VkImageLayout> _swapchainImageLayouts;

public:

    explicit Renderer(const CommandBuffers& command_buffers, const GraphicsPipeline* pipeline, const PresentSwapchain* swapchain, const LogicalDevice* device, VkQueue graphics, VkQueue present);

    void DrawFrame();

    ~Renderer();

private:

    void CreateSyncObjects();

    void RecordCommandBuffer(VkCommandBuffer buffer, VkImageView imageView, uint32_t imageIndex);

    //void CreateBuffers();

};