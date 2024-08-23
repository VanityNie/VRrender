//
// Created by win111 on 2024/7/4.
//

#include "Device.h"

Device::Device(VulkanContext *context):m_context(context) {
    
  
    VmaAllocatorCreateInfo allocatorInfo = {};
    allocatorInfo.physicalDevice = m_context->get_physcial_device();
    allocatorInfo.device = device;
    allocatorInfo.instance = m_context->get_instance();

    check(vmaCreateAllocator(&allocatorInfo, &m_memory_alloactor), "Failed to create VMA allocator");



}



void Device::create_device() {


    VkPhysicalDevice physical_device = m_context->get_physcial_device();
    
    assert(physical_device);
    VkDeviceCreateInfo device_create_info{};
    



    
    //todo finish create info

    //1.queue

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};

    QueueFamilyIndices indices = m_context->get_queue_indices();

    std::set<uint32_t> unique_queue_families = {
        indices.compute_family.has_value(),
        indices.gfx_family.has_value(),
        indices.present_family.has_value()
    };
    


    const float defaultQueuePriority(1.0f);


    for (int32_t queue_family_idx : unique_queue_families)
    {

        VkDeviceQueueCreateInfo queue_CI{};
        queue_CI.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_CI.queueFamilyIndex = queue_family_idx;
        queue_CI.queueCount = 1;
        queue_CI.pQueuePriorities = &defaultQueuePriority;
        queueCreateInfos.push_back(queue_CI);
    }

    
    device_create_info.pQueueCreateInfos = queueCreateInfos.data();
    device_create_info.queueCreateInfoCount = queueCreateInfos.size();


    device_create_info.enabledExtensionCount = device_extensions.size();
    device_create_info.ppEnabledExtensionNames = device_extensions.data();

    device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    device_create_info.pNext = device_feature_manager.get_feature_chain();

  


    if (m_context->enable_validation_layers)
    {

        
        device_create_info.enabledLayerCount = m_context->validation_layers_lst.size();
        device_create_info.ppEnabledLayerNames = m_context->validation_layers_lst.data();

    }



    check(vkCreateDevice(physical_device, &device_create_info, nullptr, &device), "Failed to create logical device");
    LOG_INFO("create Device success");
    

    // Do get Queue
    queues.resize(3, {});

    vkGetDeviceQueue(device, indices.gfx_family.value(), 0, &queues[(int)QueueType::GFX]);
    vkGetDeviceQueue(device, indices.compute_family.value(), 0,&queues[(int)QueueType::COMPUTE]);
    vkGetDeviceQueue(device, indices.present_family.value(), 0, &queues[(int)QueueType::PRESENT]);

}

