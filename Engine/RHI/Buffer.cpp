//
// Created by win111 on 2024/4/29.
//

#include "Buffer.h"

void Buffer::Create()
{
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = m_size;
    bufferInfo.usage = m_usage;
}

void Buffer::Create(const VmaAllocator& allocator)
{

    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = m_size;
    bufferInfo.usage = m_usage;

    VmaAllocationCreateInfo allocInfo = {};
    allocInfo.usage =  m_memoryUsage;

    check(
     vmaCreateBuffer(allocator, &bufferInfo, &allocInfo, &handle, &m_allocation, nullptr),"Fail to create buffer");


}

void Buffer::Destroy(const VmaAllocator& allocator)
{
    if (handle != VK_NULL_HANDLE)
    {
        vmaDestroyBuffer(allocator, handle, m_allocation);
        handle = VK_NULL_HANDLE;
        m_allocation = VK_NULL_HANDLE;
    }
}


Buffer::Buffer(const Device* device, VkDeviceSize size, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage):m_size(size),
m_usage(usage),m_memoryUsage(memoryUsage)
{

    m_device = device;
    Create(device->m_memory_alloactor);
}

void Buffer::CopyToBuffer(const void* data, VkDeviceSize size)
{
    if (size > m_size) {
        throw std::runtime_error("Copy size larger than buffer size");
    }

    void* mappedData = Map();
    memcpy(mappedData, data, static_cast<size_t>(size));
    Unmap();
}

void Buffer::CopyFromBuffer(void* data, VkDeviceSize size) const
{
    if (size > m_size) {
        throw std::runtime_error("Copy size larger than buffer size");
    }
    void* mappedData = const_cast<Buffer*>(this)->Map();
    memcpy(data, mappedData, static_cast<size_t>(size));
    const_cast<Buffer*>(this)->Unmap();
}

void Buffer::Fill(uint32_t value)
{
    void* mappedData = Map();
    std::fill_n(static_cast<uint32_t*>(mappedData), m_size / sizeof(uint32_t), value);
    Unmap();

}

void Buffer::CopyTo(VkCommandBuffer cmdBuffer, Buffer& dstBuffer, VkDeviceSize size) const
{
    if (size > m_size || size > dstBuffer.m_size) {
        throw std::runtime_error("Copy size larger than buffer size");
    }
    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    
    vkCmdCopyBuffer(cmdBuffer, handle, dstBuffer.handle, 1, &copyRegion);

}

void* Buffer::Map()
{
    void* data;
    vmaMapMemory(m_device->m_memory_alloactor, m_allocation, &data);
    return data;
}

void Buffer::Unmap()
{
    vmaUnmapMemory(m_device->m_memory_alloactor, m_allocation);
}

Buffer::~Buffer()
{
    Destroy(m_device->m_memory_alloactor);
}
