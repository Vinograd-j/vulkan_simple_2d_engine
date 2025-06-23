#pragma once

#include <vulkan/vulkan_core.h>
#include "pipeline-layout.h"

class Pipeline
{

protected:

    VkPipeline _pipeline {};

    PipelineLayout _pipelineLayout;

protected:

    VkDevice _device;

public:

    explicit Pipeline(const PipelineLayout& pipelineLayout);

    virtual ~Pipeline();

public:

    VkPipeline GetPipeline() const { return _pipeline; }

    VkPipelineLayout GetPipelineLayout() const { return  _pipelineLayout.GetPiplineLayout(); }

private:

    virtual void CreatePipeline() = 0;

};