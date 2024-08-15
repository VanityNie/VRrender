cmake_minimum_required(VERSION 3.4.0)

project(VRender VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")



if(CMAKE_BUILD_TYPE MATCHES "Debug")
    message("Enabling vulkan validation layers")
    add_compile_definitions(USEVKVALIDATIONLAYER _DEBUG)
endif()




find_package(Vulkan REQUIRED)
find_package(Threads REQUIRED)

find_package(Vulkan REQUIRED COMPONENTS shaderc_combined)
if(${Vulkan_shaderc_combined_FOUND})
    message("shaderc found")

endif()




add_subdirectory(submodules/glfw)
add_subdirectory(submodules/glm)
add_subdirectory(submodules/spirv-cross)
add_subdirectory(submodules/imgui)
add_subdirectory(submodules/VulkanMemoryAllocator)
add_subdirectory(submodules/spdlog)
add_subdirectory(Engine)
add_subdirectory(examples)