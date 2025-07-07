#pragma once
#include <vector>

#include "vulkan/vulkan.h"

class DescriptorSetLayout
{

private:

    VkDescriptorSetLayout _layout {};

    std::vector<VkDescriptorSetLayoutBinding> _bindings {};

    const VkDevice* const _device;

public:

    explicit DescriptorSetLayout(const std::vector<VkDescriptorSetLayoutBinding>& bindings, const VkDevice* device);

    void AddBinding(const VkDescriptorSetLayoutBinding& binding);

    VkDescriptorSetLayout GetDescriptorLayout() const { return _layout; }

    ~DescriptorSetLayout();

private:

    void CreateDescriptorLayout();

};
