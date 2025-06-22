#include "../include/image-views.h"

#include <stdexcept>

ImageViews::ImageViews(const VkImageSubresourceRange& subresourceRange, const VkComponentMapping& components, const std::vector<VkImage>& images, const VkDevice& device, const VkFormat& format) :
                                                                                                                                            _subresourceRange(subresourceRange),
                                                                                                                                            _components(components),
                                                                                                                                            _images(images),
                                                                                                                                            _format(format),
                                                                                                                                            _device(device)
{
    CreateImageViews();
}

void ImageViews::CreateImageViews()
{
    for (int i = 0; i < _imageViews.size(); ++i)
    {
        VkImageViewCreateInfo createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = _images[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = _format;
        createInfo.components = _components;
        createInfo.subresourceRange = _subresourceRange;

        if (vkCreateImageView(_device, &createInfo, nullptr, &_imageViews[i]) != VK_SUCCESS)
            throw std::runtime_error("failed to create image view");
    }
}

ImageViews::~ImageViews()
{
    for (const auto& imageView : _imageViews)
        vkDestroyImageView(_device, imageView, nullptr);
}
