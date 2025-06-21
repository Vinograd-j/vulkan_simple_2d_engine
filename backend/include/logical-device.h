#pragma once

#include "physical-device.h"

class LogicalDevice
{

private:

    VkDevice _device {};

private:

    PhysicalDevice _physicalDevice;

    std::map<FamilyType, VkQueue> _queues;

public:

    explicit LogicalDevice(const PhysicalDevice& physicalDevice);

    ~LogicalDevice();

private:

    void CreateLogicalDevice();

};
