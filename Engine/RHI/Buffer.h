//
// Created by win111 on 2024/4/29.
//

#ifndef VULKANABSTRACTIONLAYER_BUFFER_H
#define VULKANABSTRACTIONLAYER_BUFFER_H


#include "pch.h"


class Buffer {


private:
    VkBuffer handle;
    VkDeviceSize m_size;
    

    VkBufferUsageFlags m_usage;
    VmaMemoryUsage m_memoryUsage;

    std::string name;
    VmaAllocation m_allocation;
    VmaAllocator m_allocator;
    const Device* m_device;


    void Destroy(const VmaAllocator& allocator);
    void Create();
    void Create(const VmaAllocator& alloctor);
    

public:

    Buffer(const Device * device, VkDeviceSize size, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);


    //copy daya to buffer
    void CopyToBuffer(const void* data, VkDeviceSize size);

    //copy data from buffer
    void CopyFromBuffer(void* data, VkDeviceSize size) const;

    //fill buffer with spec val

    void Fill(uint32_t value);


    //copy buffer in device
    void CopyTo(VkCommandBuffer cmdBuffer, Buffer& dstBuffer, VkDeviceSize size) const;
    

    void* Map();
    void Unmap();

    ~Buffer();

    VkBuffer GetBuffer() const { return handle; }
    VkDeviceSize GetSize() const { return m_size; }
    void SetName(const std::string & name){
        this->name = name;
    }


};


#endif //VULKANABSTRACTIONLAYER_BUFFER_H
