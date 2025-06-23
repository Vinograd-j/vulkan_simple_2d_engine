#pragma once

#include <vulkan/vulkan_core.h>
#include <vector>

class PipelineLayout
{

private:

    VkPipelineLayout _pipelineLayout {};

    VkDevice _device;

private:

    std::vector<VkDescriptorSetLayout> _descriptorSetLayouts;

    std::vector<VkPushConstantRange> _pushConstants;

public:

    explicit PipelineLayout(const std::vector<VkDescriptorSetLayout>& descriptorSetLayout, const std::vector<VkPushConstantRange>& pushConstants, const VkDevice& device);

    VkPipelineLayout GetPiplineLayout() const { return _pipelineLayout; }

private:

    void CreatePipelineLayout();

};