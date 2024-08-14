//
// Created by win111 on 2024/4/29.
//


#include "RHI/Windows.h"
#include "RHI/VulkanContext.h"
#include"RHI/Device.h"
#include"shaderc/shaderc.hpp"
int main()
{

    shaderc::Compiler compiler;
    const char* source_text = R"glsl(
        #version 450 core
        layout(location = 0) in vec3 inPosition;
        void main() {
            gl_Position = vec4(inPosition, 1.0);
        }
    )glsl";

    shaderc::CompileOptions options;
    shaderc::SpvCompilationResult binary;
    std::string error_message;


    Logger::init();
    Windows windows;
    VulkanContext vulkan_context{&windows,true};
    Device device(&vulkan_context);
    device.create_device();
    
    
    windows.loop();
    


    


    LOG_INFO("over");
    return 0;
}