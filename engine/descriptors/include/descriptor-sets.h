#pragma once

#include <vector>

#include "descriptor-set-layout.h"
#include "vulkan/vulkan.h"

enum DescriptorKind
{
    Buffer,
    Image
};

struct DescriptorBinding
{
    uint32_t _binding;
    VkDescriptorType _type;
    DescriptorKind _kind;
    uint32_t _descriptorCount;
};

struct DescriptorResource
{
    DescriptorBinding _layout;

    std::vector<VkDescriptorBufferInfo> bufferInfos;
    std::vector<VkDescriptorImageInfo> imageInfos;
};

class DescriptorSets
{

private:

    VkDescriptorPool _pool {};

    DescriptorSetLayout _layout;

    std::vector<VkDescriptorSet> _descriptorsSet;

    const VkDevice* const _device;

public:

    explicit DescriptorSets(const VkDevice* device, const DescriptorSetLayout& layout, const std::vector<DescriptorResource>& resources, uint32_t count);

private:

    void CreateDescriptorsPool(uint32_t descriptorCount, const std::vector<DescriptorResource>& resources, uint32_t setCount);
    void CreateDescriptorSets(const std::vector<DescriptorResource>& resources, uint32_t count);

};