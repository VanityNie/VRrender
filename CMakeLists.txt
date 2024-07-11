cmake_minimum_required(VERSION 3.4.0)


if(CMAKE_BUILD_TYPE MATCHES "Debug")
    message("Enabling vulkan validation layers")
    add_compile_definitions(USEVKVALIDATIONLAYER _DEBUG)
endif()




add_compile_definitions(TINYEXR_USE_MINIZ)  # enables usage of miniz for opening exr images
add_compile_definitions(USE_SHADERC)        # enables shader compilation via shaderc



find_package(Vulkan REQUIRED)
find_package(Threads REQUIRED)