add_library(backend STATIC
        src/Instance.cpp
        include/Instance.h
        src/vulkan-application.cpp
        include/vulkan-application.h
        include/window.h
        src/window.cpp
        src/surface.cpp
        include/surface.h
        include/physical-device.h
        src/physical-device.cpp
        include/logical-device.h
        src/logical-device.cpp
        include/allocator.h
        src/allocator.cpp
        include/queue-family-indices.h
        include/swap-chain-support-details.h
)

target_include_directories(backend PUBLIC include)

target_link_libraries(backend PUBLIC Vulkan::Vulkan GPUOpen::VulkanMemoryAllocator glad glfw)