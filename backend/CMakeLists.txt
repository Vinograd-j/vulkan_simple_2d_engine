add_library(backend STATIC
        src/instance.cpp
        include/instance.h
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
        src/validation-layer-messenger.cpp
        include/validation-layer-messenger.h
        include/queue-family-indices.h
        include/swap-chain-support-details.h

)

target_include_directories(backend PUBLIC include)

target_link_libraries(backend PRIVATE Vulkan::Vulkan GPUOpen::VulkanMemoryAllocator glad glfw)