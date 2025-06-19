#pragma once

#include <string>
#include <utility>
#include "vulkan/vulkan_core.h"

class VulkanApplication
{

private:

    std::string _appName;

    uint32_t _version;

    std::string _engineName;

    uint32_t _engineVersion;

    std::uint32_t _apiVersion;

private:

    VkApplicationInfo _appInfo {};

private:

    void CreateApplicationInfo(void* pNext);

public:

    VulkanApplication(std::string appName, uint32_t version, std::string engineName, uint32_t engineVersion, std::uint32_t apiVersion, void* pNext);

    VkApplicationInfo* GetApplicationInfo() { return &_appInfo; }

};