#pragma once

#include "vulkan-application.h"
#include "vulkan/vulkan.hpp"

class instance
{

private:

    VkInstance _instance {};

private:

    VulkanApplication _application;

    void* _pNext;

    std::vector<const char*> _extensions;

    std::vector<const char*> _layers;

    VkInstanceCreateFlags _flags;

public:

    explicit instance(const VulkanApplication& application, const std::vector<const char*>& extensions, const std::vector<const char*> layers, void* pNext, const VkInstanceCreateFlags& flags) :
                                                                                                                                                             _application(application),
                                                                                                                                                             _pNext(pNext),
                                                                                                                                                             _extensions(extensions),
                                                                                                                                                             _layers(layers),
                                                                                                                                                             _flags(flags) {}

    void CreateInstance();

public:

    const VkInstance* GetInstance() const { return &_instance; }

    void* GetPNext() const { return _pNext; }

};