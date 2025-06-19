#pragma once

#include "queue-family-indices.h"
#include "Vulkan/vulkan.hpp"

#include  "swap-chain-support-details.h"

class PhysicalDevice
{

private:

    VkPhysicalDevice _device {};

    QueueFamilyIndices _familyIndices;

private:

    VkInstance* _instance;

    VkSurfaceKHR* _surface;

    std::vector<const char*> _extensions;

public:

    explicit PhysicalDevice(VkInstance* instance, VkSurfaceKHR* surface, const std::vector<const char*>& requiredExtensions) : _instance(instance), _surface(surface), _extensions(requiredExtensions) {}

    void ChoosePhysicalDevice(uint32_t deviceID);

    VkPhysicalDevice* GetPhysicalDevice() { return &_device; }

    SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& device) const;

private:

    QueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice& device) const;

    bool IsDeviceSuitable(const VkPhysicalDevice& device) const;

    bool CheckDeviceExtensionsSupport(const VkPhysicalDevice& device) const;

};