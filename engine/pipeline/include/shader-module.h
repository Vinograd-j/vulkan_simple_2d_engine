#pragma once

#include "logical-device.h"
#include "../../../file-manager/include/file-reader.h"

class ShaderModule
{

private:

    FileReader _fileReader;

    VkDevice _device {};

    VkShaderStageFlagBits _stage {};

    std::string _shaderName;

private:

    VkShaderModule _shaderModule {};
    VkPipelineShaderStageCreateInfo _shaderStage {};

public:

    explicit ShaderModule(const VkDevice& device, const VkPipelineShaderStageCreateInfo& shaderStage, const std::string& shaderName);
    ~ShaderModule();

    VkShaderModule GetShaderModule() const { return _shaderModule; }
    VkPipelineShaderStageCreateInfo GetShaderStage() const { return _shaderStage; }

private:

    void CreateShaderModule();
    void CreateShaderStage();

};