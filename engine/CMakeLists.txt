add_library(engine
        swapchain/src/present-swapchain.cpp
        swapchain/include/present-swapchain.h
        swapchain/src/i-swap-chain.cpp
        swapchain/include/i-swap-chain.h
)

target_include_directories(engine PUBLIC include)
target_link_libraries(engine backend Vulkan::Vulkan)