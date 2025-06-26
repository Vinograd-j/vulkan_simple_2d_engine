#include "../include/descriptor-set-layout.h"

#include <stdexcept>

DescriptorSetLayout::DescriptorSetLayout(const std::vector<VkDescriptorSetLayoutBinding>& bindings, const VkDevice* device) : _bindings(bindings), _device(device)
{
    CreateDescriptorLayout();
}

void DescriptorSetLayout::CreateDescriptorLayout()
{
    VkDescriptorSetLayoutCreateInfo createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    createInfo.bindingCount = _bindings.size();
    createInfo.pBindings = _bindings.data();

    if (vkCreateDescriptorSetLayout(*_device, &createInfo, nullptr, &_layout) != VK_SUCCESS)
        throw std::runtime_error("failed to create descriptor set layout");
}

void DescriptorSetLayout::AddBinding(const VkDescriptorSetLayoutBinding& binding)
{

    vkDestroyDescriptorSetLayout(*_device, _layout, nullptr);

    _bindings.push_back(binding);

    CreateDescriptorLayout();
}

DescriptorSetLayout::~DescriptorSetLayout()
{
    vkDestroyDescriptorSetLayout(*_device, _layout, nullptr);
}