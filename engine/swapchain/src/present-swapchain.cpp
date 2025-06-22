#include "../include/present-swapchain.h"

PresentSwapchain::PresentSwapchain(const PhysicalDevice& physicalDevice, const LogicalDevice& device, int width, int height, const VkSurfaceKHR& surface) :
                                                                                                                                                            ISwapchain(physicalDevice, device, width, height, surface)
{
    PresentSwapchain::CreateSwapchain();
}

void PresentSwapchain::CreateSwapchain()
{
    ChooseSurfaceFormat();
    ChooseExtent();
    ChoosePresentMode();

    uint32_t imageCount = _swapChainSupportDetails._capabilities.minImageCount + 1;

    if (_swapChainSupportDetails._capabilities.maxImageCount > 0 && imageCount > _swapChainSupportDetails._capabilities.maxImageCount)
        imageCount = _swapChainSupportDetails._capabilities.maxImageCount;

    VkSwapchainCreateInfoKHR createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = _surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = _surfaceFormat.format;
    createInfo.imageColorSpace = _surfaceFormat.colorSpace;
    createInfo.imageExtent = _extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    auto indices = _physicalDevice.GetFamilies(_physicalDevice.GetPhysicalDevice());
    uint32_t queueFamilyIndices[] = {indices.at(GRAPHICS), indices.at(PRESENT)};

    if (indices.at(GRAPHICS) != indices.at(PRESENT))
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = _swapChainSupportDetails._capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = _presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(_device.GetDevice(), &createInfo, nullptr, &_swapchain) != VK_SUCCESS)
        throw std::runtime_error("failed to create swapchain");

    vkGetSwapchainImagesKHR(_device.GetDevice(), _swapchain, &imageCount, nullptr);
    _swapchainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(_device.GetDevice(), _swapchain, &imageCount, _swapchainImages.data());

}

void PresentSwapchain::ChooseExtent()
{
    if (_swapChainSupportDetails._capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        _extent = _swapChainSupportDetails._capabilities.currentExtent;

    VkExtent2D actualExtent = {
        static_cast<uint32_t>(_width),
        static_cast<uint32_t>(_height)
    };

    actualExtent.width = std::clamp(actualExtent.width, _swapChainSupportDetails._capabilities.minImageExtent.width, _swapChainSupportDetails._capabilities.maxImageExtent.width);
    actualExtent.height = std::clamp(actualExtent.height, _swapChainSupportDetails._capabilities.minImageExtent.height, _swapChainSupportDetails._capabilities.maxImageExtent.height);

    _extent = actualExtent;
}

void PresentSwapchain::ChooseSurfaceFormat()
{
    for (const auto& availableFormat : _swapChainSupportDetails._formats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            _surfaceFormat = availableFormat;
            return;
        }
    }

    _surfaceFormat = _swapChainSupportDetails._formats[0];
}

void PresentSwapchain::ChoosePresentMode()
{
    for (const auto& availablePresentMode : _swapChainSupportDetails._presentModes)
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            _presentMode = availablePresentMode;
            return;
        }
    }

    _presentMode = VK_PRESENT_MODE_FIFO_KHR;
}

void PresentSwapchain::CleanupSwapchain()
{
    for (const auto& image: _swapchainImages)
        vkDestroyImage(_device.GetDevice(), image, nullptr);

    vkDestroySwapchainKHR(_device.GetDevice(), _swapchain, nullptr);
}

std::vector<VkImageView> PresentSwapchain::GetImageViews(const VkImageSubresourceRange& subresourceRange, const VkComponentMapping& components) const
{
    std::vector<VkImageView> imageViews(_swapchainImages.size());

    for (int i = 0; i < imageViews.size(); ++i)
    {
        VkImageViewCreateInfo createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = _swapchainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = _surfaceFormat.format;
        createInfo.components = components;
        createInfo.subresourceRange = subresourceRange;

        if (vkCreateImageView(_device.GetDevice(), &createInfo, nullptr, &imageViews[i]) != VK_SUCCESS)
            throw std::runtime_error("failed to create image view");
    }

    return imageViews;
}

PresentSwapchain::~PresentSwapchain()
{
    PresentSwapchain::CleanupSwapchain();
}