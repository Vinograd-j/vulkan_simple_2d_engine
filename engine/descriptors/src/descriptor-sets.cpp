#include "../include/descriptor-sets.h"

#include <stdexcept>

DescriptorSets::DescriptorSets(const VkDevice* device, const DescriptorSetLayout& layout, const std::vector<DescriptorResource>& resources, uint32_t count) : _layout(layout),
                                                                                                                                                              _device(device)
{
    CreateDescriptorsPool(count, resources, count);
    CreateDescriptorSets(resources, count);
}

void DescriptorSets::CreateDescriptorsPool(uint32_t descriptorCount, const std::vector<DescriptorResource>& resources, uint32_t setCount)
{
    std::vector<VkDescriptorPoolSize> poolSizes;

    for (const auto& resource : resources)
    {
        VkDescriptorPoolSize size {};
        size.type = resource._layout._type;
        size.descriptorCount = resource._layout._descriptorCount * setCount;

        poolSizes.push_back(size);
    }

    VkDescriptorPoolCreateInfo createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    createInfo.poolSizeCount = poolSizes.size();
    createInfo.pPoolSizes = poolSizes.data();
    createInfo.maxSets = descriptorCount;

    if (vkCreateDescriptorPool(*_device, &createInfo, nullptr, &_pool) != VK_SUCCESS)
        throw std::runtime_error("failed to create descriptor pool");
}

void DescriptorSets::CreateDescriptorSets(const std::vector<DescriptorResource>& resources, uint32_t count)
{
    std::vector<VkDescriptorSetLayout> layouts(count, _layout.GetDescriptorLayout());

    VkDescriptorSetAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = _pool;
    allocInfo.descriptorSetCount = count;
    allocInfo.pSetLayouts = layouts.data();

    std::vector<VkDescriptorSet> sets(count);
    if (vkAllocateDescriptorSets(*_device, &allocInfo, sets.data()) != VK_SUCCESS)
        throw std::runtime_error("failed to allocate descriptor sets");

    for (int i = 0; i < count; ++i)
    {
        std::vector<VkWriteDescriptorSet> writes;

        for (const auto& resource: resources)
        {
            VkWriteDescriptorSet write {};
            write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            write.dstSet = sets[i];
            write.dstBinding = resource._layout._binding;
            write.dstArrayElement = 0;
            write.descriptorType = resource._layout._type;
            write.descriptorCount = resource._layout._descriptorCount;

            if (resource._layout._kind == Buffer)
                write.pBufferInfo = resource.bufferInfos.data() + i * resource._layout._descriptorCount;
            else if (resource._layout._kind == Image)
                write.pImageInfo = resource.imageInfos.data() + i * resource._layout._descriptorCount;

            writes.push_back(write);
        }

        vkUpdateDescriptorSets(*_device, writes.size(), writes.data(), 0, nullptr);
    }

    _descriptorsSet = sets;
}