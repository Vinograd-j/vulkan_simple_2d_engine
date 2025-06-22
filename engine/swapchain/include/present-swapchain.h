#pragma once

#include "i-swap-chain.h"
#include "vulkan/vulkan.h"

class PresentSwapchain : ISwapchain
{

public:

    explicit PresentSwapchain(const PhysicalDevice& physicalDevice, const LogicalDevice& device, int width, int height, const VkSurfaceKHR& surface);

    ~PresentSwapchain();

    size_t GetImageCount() const override { return _swapchainImages.size(); }

private:

    void CreateSwapchain() override;

public:

    void CleanupSwapchain() override;

private:

    void ChooseExtent();
    void ChooseSurfaceFormat();
    void ChoosePresentMode();

};