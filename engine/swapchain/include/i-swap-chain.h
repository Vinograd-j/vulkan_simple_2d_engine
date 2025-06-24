#pragma once
#include "logical-device.h"
#include "swap-chain-support-details.h"

class ISwapchain
{

protected:

    VkSwapchainKHR _swapchain {};

    VkPresentModeKHR _presentMode {};
    VkSurfaceFormatKHR _surfaceFormat {};
    VkExtent2D _extent {};

    std::vector<VkImage> _swapchainImages;

    SwapChainSupportDetails _swapChainSupportDetails {};

protected:

    PhysicalDevice _physicalDevice;
    const LogicalDevice* _device;
    VkSurfaceKHR _surface;

    int _width;
    int _height;

public:

    explicit ISwapchain(const PhysicalDevice& physicalDevice, const LogicalDevice* device, int width, int height, const VkSurfaceKHR& surface);

    virtual ~ISwapchain() = default;

public:

    virtual size_t GetImageCount() const = 0;
    virtual VkExtent2D GetExtent() const { return _extent; }
    virtual VkFormat GetImageFormat() const { return _surfaceFormat.format; }
    virtual void CleanupSwapchain() = 0;

    virtual std::vector<VkImageView> GetImageViews(const VkImageSubresourceRange& subresourceRange, const VkComponentMapping& components) const = 0;

private:

    virtual void CreateSwapchain(const VkSwapchainKHR& oldSwapchain) = 0;

};