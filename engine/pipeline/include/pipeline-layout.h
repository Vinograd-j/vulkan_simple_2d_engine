#pragma once

#include <vulkan/vulkan_core.h>
#include <vector>

#include "logical-device.h"

class PipelineLayout
{

private:

    VkPipelineLayout _pipelineLayout {};

    const LogicalDevice* const _device;

private:

    std::vector<VkDescriptorSetLayout> _descriptorSetLayouts;

    std::vector<VkPushConstantRange> _pushConstants;

public:

    explicit PipelineLayout(const std::vector<VkDescriptorSetLayout>& descriptorSetLayout, const std::vector<VkPushConstantRange>& pushConstants, const LogicalDevice* device);

    VkPipelineLayout GetPiplineLayout() const { return _pipelineLayout; }

    ~PipelineLayout();

private:

    void CreatePipelineLayout();

};
