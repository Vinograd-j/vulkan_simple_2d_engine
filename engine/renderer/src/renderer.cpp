#include "../include/renderer.h"

#include "../buffers/include/index-buffer.h"

Renderer::Renderer(const CommandPool* pool, const CommandBuffers& command_buffers, const GraphicsPipeline* pipeline, PresentSwapchain* swapchain, const LogicalDevice* device, const Allocator* allocator)  :
                                                                        _squareDrawer(pool, command_buffers, pipeline, swapchain, device, allocator) {}

void Renderer::DrawFrame()
{
    _squareDrawer.DrawFrame();
}