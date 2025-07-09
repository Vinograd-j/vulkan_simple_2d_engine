#pragma once

#include "allocator.h"
#include "square-drawer.h"
#include "synchronization.h"
#include "../../pipeline/include/graphics-pipeline.h"
#include "../../swapchain/include/present-swapchain.h"
#include "../command-buffer/include/command-buffers.h"
#include "../command-buffer/include/command-pool.h"

class Renderer
{

private:

    SquareDrawer _squareDrawer;

public:

    explicit Renderer(const CommandPool* pool, const CommandBuffers& command_buffers, const GraphicsPipeline* pipeline, PresentSwapchain* swapchain, const LogicalDevice* device, const Allocator* allocator);

    void DrawFrame();

};