add_library(engine
        swapchain/src/present-swapchain.cpp
        swapchain/include/present-swapchain.h
        swapchain/src/i-swap-chain.cpp
        swapchain/include/i-swap-chain.h
        pipeline/include/pipeline.h
        pipeline/src/pipeline-layout.cpp
        pipeline/include/pipeline-layout.h
        pipeline/src/graphics-pipeline.cpp
        pipeline/include/graphics-pipeline.h
        pipeline/src/shader-module.cpp
        pipeline/include/shader-module.h
        struct/vertex.h
        descriptors/src/descriptor-set-layout.cpp
        descriptors/include/descriptor-set-layout.h
        descriptors/src/descriptor-sets.cpp
        descriptors/include/descriptor-sets.h
        renderer/src/renderer.cpp
        renderer/include/renderer.h
        renderer/command-buffer/src/command-buffers.cpp
        renderer/command-buffer/include/command-buffers.h
        renderer/command-buffer/src/command-pool.cpp
        renderer/command-buffer/include/command-pool.h
)

target_include_directories(engine PUBLIC include)
target_link_libraries(engine backend Vulkan::Vulkan file-manager)