#include "../include/renderer.h"

Renderer::Renderer(const CommandBuffers& command_buffers, const GraphicsPipeline* pipeline, const PresentSwapchain* swapchain, const LogicalDevice* device, VkQueue graphics, VkQueue present, const Allocator* allocator)  :
                                                                                                                                                          _commandBuffers(command_buffers),
                                                                                                                                                          _pipeline(pipeline),
                                                                                                                                                          _swapchain(swapchain),
                                                                                                                                                          _device(device),
                                                                                                                                                          _graphicsQueue(graphics),
                                                                                                                                                          _presentQueue(present),
                                                                                                                                                          _allocator(allocator)

{
    CreateBuffers();
    CreateSyncObjects();

    VkComponentMapping mapping {};
    mapping.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    mapping.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    mapping.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    mapping.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    VkImageSubresourceRange subresourceRange {};
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = 1;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = 1;

    _imageViews = _swapchain->GetImageViews(subresourceRange, mapping);

    _swapchainImageLayouts.resize(_swapchain->GetSwapchainImages().size(), VK_IMAGE_LAYOUT_UNDEFINED);
}

void Renderer::DrawFrame()
{
    vkWaitForFences(_device->GetDevice(), 1, &_inFlightFences[_currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(_device->GetDevice(), _swapchain->GetSwapchain(), UINT64_MAX, _imageAvailableSemaphores[_currentFrame], VK_NULL_HANDLE, &imageIndex);

    vkResetFences(_device->GetDevice(), 1, &_inFlightFences[_currentFrame]);

    vkResetCommandBuffer(_commandBuffers[_currentFrame], 0);

    RecordCommandBuffer(_commandBuffers[_currentFrame], _imageViews[imageIndex], imageIndex);

    VkSubmitInfo submitInfo {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {_imageAvailableSemaphores[_currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &_commandBuffers[_currentFrame];

    VkSemaphore signalSemaphores[] = {_renderingFinishedSemaphores[_currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(_graphicsQueue, 1, &submitInfo, _inFlightFences[_currentFrame]) != VK_SUCCESS)
        throw std::runtime_error("failed to submit draw command buffer");

    VkPresentInfoKHR presentInfo {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {_swapchain->GetSwapchain() };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(_presentQueue, &presentInfo);

    // if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _framebufferResized)
    // {
    //     _framebufferResized = false;
    //     RecreateSwapChain();
    //     return;
    // }else if (result != VK_SUCCESS)
    // {
    //     throw std::runtime_error("failed to acquire swap chain image");
    // }

    _currentFrame = (_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Renderer::RecordCommandBuffer(VkCommandBuffer buffer, VkImageView imageView, uint32_t imageIndex)
{
    VkCommandBufferBeginInfo beginInfo {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = nullptr;

    if (vkBeginCommandBuffer(buffer, &beginInfo) != VK_SUCCESS)
        throw std::runtime_error("failed to begin command buffer");

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};

    VkRenderingAttachmentInfo colorAttachmentInfo{};
    colorAttachmentInfo.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    colorAttachmentInfo.imageView = imageView;
    colorAttachmentInfo.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    colorAttachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachmentInfo.clearValue = clearColor;

    VkRenderingInfo renderingInfo {};
    renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
    renderingInfo.renderArea.offset = {0, 0};
    renderingInfo.renderArea.extent = _swapchain->GetExtent();
    renderingInfo.layerCount = 1;
    renderingInfo.colorAttachmentCount = 1;
    renderingInfo.pColorAttachments = &colorAttachmentInfo;

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(_swapchain->GetExtent().width);
    viewport.height = static_cast<float>(_swapchain->GetExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(buffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = _swapchain->GetExtent();
    vkCmdSetScissor(buffer, 0, 1, &scissor);

    vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline->GetPipeline());

    VkImageMemoryBarrier barrierToRender{};
    barrierToRender.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrierToRender.oldLayout = _swapchainImageLayouts[imageIndex];
    barrierToRender.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    barrierToRender.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrierToRender.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrierToRender.image = _swapchain->GetSwapchainImages()[imageIndex];
    barrierToRender.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrierToRender.subresourceRange.baseMipLevel = 0;
    barrierToRender.subresourceRange.levelCount = 1;
    barrierToRender.subresourceRange.baseArrayLayer = 0;
    barrierToRender.subresourceRange.layerCount = 1;

    barrierToRender.srcAccessMask = 0;
    barrierToRender.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    _swapchainImageLayouts[imageIndex] = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    vkCmdPipelineBarrier(
        buffer,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrierToRender
    );

    vkCmdBeginRendering(buffer, &renderingInfo);

    VkBuffer vertexBuffers[] = {_vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(buffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(buffer, _indexBuffer, 0, VK_INDEX_TYPE_UINT16);

    vkCmdDrawIndexed(buffer, indices.size(), 1, 0, 0, 0);

    vkCmdEndRendering(buffer);

    VkImageMemoryBarrier barrierToPresent{};
    barrierToPresent.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrierToRender.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    barrierToPresent.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    barrierToPresent.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrierToPresent.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrierToPresent.image = _swapchain->GetSwapchainImages()[imageIndex];
    barrierToPresent.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrierToPresent.subresourceRange.baseMipLevel = 0;
    barrierToPresent.subresourceRange.levelCount = 1;
    barrierToPresent.subresourceRange.baseArrayLayer = 0;
    barrierToPresent.subresourceRange.layerCount = 1;

    barrierToPresent.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    barrierToPresent.dstAccessMask = 0;

    _swapchainImageLayouts[imageIndex] = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    vkCmdPipelineBarrier(
        buffer,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrierToPresent
    );


    vkEndCommandBuffer(buffer);
}

void Renderer::CreateBuffers()
{
    VkDeviceSize vertexBufferSize = sizeof(vertices[0]) * vertices.size();

    VkBufferCreateInfo vertexBufferInfo{};
    vertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vertexBufferInfo.size = vertexBufferSize;
    vertexBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    vertexBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo allocInfo{};
    allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
    allocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
                      VMA_ALLOCATION_CREATE_MAPPED_BIT;

    VmaAllocationInfo allocationInfo{};
    _allocator->CreateBuffer(vertexBufferInfo, allocInfo, &_vertexBuffer, &_vertexBufferMemory, &allocationInfo);

    std::memcpy(allocationInfo.pMappedData, vertices.data(), static_cast<size_t>(vertexBufferSize));


    VkDeviceSize indexBufferSize = sizeof(indices[0]) * indices.size();

    VkBufferCreateInfo indexBufferInfo{};
    indexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    indexBufferInfo.size = indexBufferSize;
    indexBufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    indexBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo indexAllocCreateInfo{};
    indexAllocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
    indexAllocCreateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
                                  VMA_ALLOCATION_CREATE_MAPPED_BIT;

    VmaAllocationInfo indexAllocInfo{};
    _allocator->CreateBuffer(indexBufferInfo, indexAllocCreateInfo, &_indexBuffer, &_indexBufferMemory, &indexAllocInfo);

    std::memcpy(indexAllocInfo.pMappedData, indices.data(), indexBufferSize);
}

void Renderer::CreateSyncObjects()
{
    _imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    _renderingFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    _inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreCreateInfo {};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceCreateInfo {};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
    {
        if (vkCreateSemaphore(_device->GetDevice(), &semaphoreCreateInfo, nullptr, &_imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(_device->GetDevice(), &semaphoreCreateInfo, nullptr, &_renderingFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(_device->GetDevice(), &fenceCreateInfo, nullptr, &_inFlightFences[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create semaphore or fence for a frame");
        }
    }
}

Renderer::~Renderer()
{
    vmaDestroyBuffer(_allocator->GetAllocator(), _vertexBuffer, _vertexBufferMemory);
    vmaDestroyBuffer(_allocator->GetAllocator(), _indexBuffer, _indexBufferMemory);

    for (auto imageView : _imageViews)
        vkDestroyImageView(_device->GetDevice(), imageView, nullptr);

    for (auto semaphore : _imageAvailableSemaphores)
        vkDestroySemaphore(_device->GetDevice(), semaphore, nullptr);

    for (auto semaphore : _renderingFinishedSemaphores)
        vkDestroySemaphore(_device->GetDevice(), semaphore, nullptr);

    for (auto fence : _inFlightFences)
        vkDestroyFence(_device->GetDevice(), fence, nullptr);
}
