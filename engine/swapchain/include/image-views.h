#pragma once
#include <vector>
#include <vulkan/vulkan_core.h>

class ImageViews
{

private:

    std::vector<VkImageView> _imageViews;

    VkImageSubresourceRange _subresourceRange;
    VkComponentMapping _components;

    std::vector<VkImage> _images;
    VkFormat _format;
    VkDevice _device;

public:

    explicit ImageViews(const VkImageSubresourceRange& subresourceRange, const VkComponentMapping& components, const std::vector<VkImage>& images, const VkDevice& device, const VkFormat& format);

    std::vector<VkImageView> GetImageViews() const { return _imageViews; }

    ~ImageViews();

private:

    void CreateImageViews();

};
