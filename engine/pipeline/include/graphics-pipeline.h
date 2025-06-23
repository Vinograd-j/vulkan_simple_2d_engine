#pragma once

#include "pipeline.h"
#include "shader-module.h"
#include "../../../file-manager/include/file-reader.h"

class GraphicsPipeline : Pipeline
{

private:

    FileReader _fileReader;

    std::vector<ShaderModule> _shaderModules;

public:

    explicit GraphicsPipeline(const std::vector<ShaderModule>& shaderModules, const PipelineLayout& pipelineLayout);

    ~GraphicsPipeline() override;

private:

    void CreatePipeline() override;

    VkPipelineVertexInputStateCreateInfo CreateVertexInputInfo() const;

    VkPipelineRasterizationStateCreateInfo CreateRasterizer() const;

    VkPipelineMultisampleStateCreateInfo SetupMultisampling() const;

    VkPipelineColorBlendAttachmentState SetupColorBlendAttachment() const;

    VkPipelineColorBlendStateCreateInfo SetupColorBlending(const VkPipelineColorBlendAttachmentState& attachmentState) const;

};
