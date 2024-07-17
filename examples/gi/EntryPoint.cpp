//
// Created by win111 on 2024/4/29.
//


#include "RHI/Windows.h"
#include "RHI/VulkanContext.h"

int main()
{
    Logger::init();
    Windows windows;
    VulkanContext vulkan_context{&windows,true};
    windows.loop();
    return 0;
}