//
// Created by win111 on 2024/4/29.
//


#include "RHI/Windows.h"
#include "RHI/VulkanContext.h"

int main()
{
    Logger::init();
    DescriptorInfo::print();
    Windows windows;
    windows.loop();
    return 0;
}