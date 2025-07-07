#include "../include/pipeline-layout.h"

#include <stdexcept>

PipelineLayout::PipelineLayout(const std::vector<VkDescriptorSetLayout>& descriptorSetLayout, const std::vector<VkPushConstantRange>& pushConstants, const LogicalDevice* device) :
                                                                                                                                                        _device(device),
                                                                                                                                                        _descriptorSetLayouts(descriptorSetLayout),
                                                                                                                                                        _pushConstants(pushConstants)
{
    CreatePipelineLayout();
}

void PipelineLayout::CreatePipelineLayout()
{
    VkPipelineLayoutCreateInfo createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    createInfo.setLayoutCount = _descriptorSetLayouts.size();
    createInfo.pSetLayouts = _descriptorSetLayouts.data();
    createInfo.pushConstantRangeCount = _pushConstants.size();
    createInfo.pPushConstantRanges = _pushConstants.data();

    if (vkCreatePipelineLayout(_device->GetDevice(), &createInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
        throw std::runtime_error("failed to create pipeline layout");
}

PipelineLayout::~PipelineLayout()
{
    vkDestroyPipelineLayout(_device->GetDevice(), _pipelineLayout, nullptr);
}