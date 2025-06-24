#include "../include/shader-module.h"

ShaderModule::ShaderModule(LogicalDevice* device, const VkShaderStageFlagBits& shaderStage, const std::string& shaderName) : _device(device), _shaderName(shaderName), _stage(shaderStage)
{
    CreateShaderModule();
    CreateShaderStage();
}

void ShaderModule::CreateShaderModule()
{
    auto shaderCode = _fileReader.ReadFromFile(std::string("shaders") + "/" + _shaderName + ".spv");

    VkShaderModuleCreateInfo createInfo {};

    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = shaderCode.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

    if (vkCreateShaderModule(_device->GetDevice(), &createInfo, nullptr, &_shaderModule) != VK_SUCCESS)
        throw std::runtime_error("failed to create shader module");
}

void ShaderModule::CreateShaderStage()
{
    VkPipelineShaderStageCreateInfo createInfo {};

    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    createInfo.stage = _stage;
    createInfo.module = _shaderModule;
    createInfo.pName = "main";

    _shaderStage = createInfo;
}

ShaderModule::~ShaderModule()
{
    vkDestroyShaderModule(_device->GetDevice(), _shaderModule, nullptr);
}