#pragma once

#include "allocator.h"
#include "buffer.h"

class IndexBuffer
{

private:

    Buffer<uint16_t> _indexBuffer;

    std::vector<uint16_t> _indices;

public:

    explicit IndexBuffer(const Allocator* allocator, const std::vector<uint16_t>& vertices, const CommandPool* pool, const LogicalDevice* device);

    std::vector<uint16_t> GetIndices() const { return _indices; }

    VkBuffer GetBuffer() const { return _indexBuffer.GetBuffer(); }

private:

    void CreateBuffer();

};
