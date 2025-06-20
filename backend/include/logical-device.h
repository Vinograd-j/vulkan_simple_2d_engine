#pragma once

#include "physical-device.h"

class LogicalDevice
{

private:

    VkDevice _device {};

private:

    PhysicalDevice _physicalDevice;

    std::map<FamilyType, VkQueue> _queues;

    VkPhysicalDeviceFeatures _features;

public:

    explicit LogicalDevice(const PhysicalDevice& physicalDevice, const VkPhysicalDeviceFeatures& features);

    ~LogicalDevice();

private:

    void CreateLogicalDevice();

};
