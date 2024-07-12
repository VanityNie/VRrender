//
// Created by win111 on 2024/7/4.
//

#ifndef VULKANABSTRACTIONLAYER_DEVICE_H
#define VULKANABSTRACTIONLAYER_DEVICE_H

#include "vulkan/vulkan.h"
#include "VulkanContext.h"
#include"vma/vk_mem_alloc.h"
class Device {


private:
    VkDevice device;
    VulkanContext * m_context;


public:
    explicit Device(VulkanContext * context );
    void create_device();


    VmaAllocator m_memory_alloactor;
};


#endif //VULKANABSTRACTIONLAYER_DEVICE_H
