#pragma once

#include "i-swap-chain.h"
#include "vulkan/vulkan.h"

class PresentSwapchain : ISwapchain
{

public:

    explicit PresentSwapchain(const PhysicalDevice& physicalDevice, LogicalDevice* device, GLFWwindow* window, const VkSurfaceKHR& surface, const VkSwapchainKHR& oldSwapchain);

    ~PresentSwapchain() override;

    size_t GetImageCount() const override { return _swapchainImages.size(); }

private:

    void CreateSwapchain(const VkSwapchainKHR& oldSwapchain) override;

public:

    void CleanupSwapchain() override;

    std::vector<VkImageView> GetImageViews(const VkImageSubresourceRange& subresourceRange, const VkComponentMapping& components) const override;

    std::vector<VkImage> GetSwapchainImages() const override { return _swapchainImages; }

    VkSwapchainKHR GetSwapchain() const { return _swapchain; }

    VkExtent2D GetExtent() const override { return _extent; };

    VkFormat GetImageFormat() const override { return _surfaceFormat.format; }

    void Recreate();

private:

    void ChooseExtent();
    void ChooseSurfaceFormat();
    void ChoosePresentMode();

};