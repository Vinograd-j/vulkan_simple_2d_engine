#include "../include/i-swap-chain.h"

ISwapchain::ISwapchain(const PhysicalDevice& physicalDevice, const LogicalDevice* device, int width, int height, const VkSurfaceKHR& surface) :
                                                                                                                                            _physicalDevice(physicalDevice),
                                                                                                                                            _device(device),
                                                                                                                                            _surface(surface),
                                                                                                                                            _width(width),
                                                                                                                                            _height(height)
{
    _swapChainSupportDetails = _physicalDevice.QuerySwapChainSupportDetails(_physicalDevice.GetPhysicalDevice());
}