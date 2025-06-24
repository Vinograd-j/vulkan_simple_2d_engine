#pragma once

#include <vulkan/vulkan_core.h>

#include "logical-device.h"
#include "pipeline-layout.h"

class Pipeline
{

protected:

    VkPipeline _pipeline {};

    const PipelineLayout* _pipelineLayout;

protected:

    LogicalDevice* _device;

public:

    explicit Pipeline(const PipelineLayout* pipelineLayout, LogicalDevice* device) : _pipelineLayout(pipelineLayout), _device(device) {}

    virtual ~Pipeline() = default;

public:

    VkPipeline GetPipeline() const { return _pipeline; }

    VkPipelineLayout GetPipelineLayout() const { return  _pipelineLayout->GetPiplineLayout(); }

private:

    virtual void CreatePipeline() = 0;

};