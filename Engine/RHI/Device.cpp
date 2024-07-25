//
// Created by win111 on 2024/7/4.
//

#include "Device.h"

Device::Device(VulkanContext *context) {
    
    m_context = context;

}

void Device::create_device() {


    VkPhysicalDevice physical_device = m_context->get_physcial_device();
    VkDeviceCreateInfo device_create_info{};


    //todo finish create info
   
    vkCreateDevice(physical_device, &device_create_info, nullptr, &device);


      
}
