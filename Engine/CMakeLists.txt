




set(ENGINE_ROOT_DIR "${ROOT_DIR}/engine")

file(GLOB RHI_SOURCE CONFIGURE_DEPENDS RHI/*.cpp RHI/*.h)

set(source ${RHI_SOURCE})

message(${source})

add_library(ENGINE STATIC ${source})

if(MSVC)
    target_compile_options(ENGINE PRIVATE "/MP")
    set_target_properties(ENGINE PROPERTIES
            VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ENGINE)
endif()



set(GLM_INCLUDE_DIR           ${ROOT_DIR}/submodules/glm)
set(GLFW_INCLUDE_DIR           ${ROOT_DIR}/submodules/glfw/include)
set(IMGUI_INCLUDE_DIR          ${ROOT_DIR}/submodules/imgui)
set(GLSLANG_INCLUDE_DIR        ${ROOT_DIR}/submodules/glslang/glslang/Public ${CMAKE_CURRENT_SOURCE_DIR}/submodules/glslang/SPIRV)
set(VMA_INCLUDE_DIR            ${ROOT_DIR}/submodules/VulkanMemoryAllocator/src)
set(TINYOBJLOADER_INCLUDE_DIR  ${ROOT_DIR}/submodules/tinyobjloader)
set(SPAN_INCLUDE_DIR           ${ROOT_DIR}/submodules/span/include)
set(SPIRV_REFLECT_INCLUDE_DIR  ${ROOT_DIR}/submodules/spirv-reflect)
set(STB_INCLUDE_DIR            ${ROOT_DIR}/submodules/stb)
set(TINYGLTF_INCLUDE_DIR       ${ROOT_DIR}/submodules/tinygltf)
set(TINYDDSLOADER_INCLUDE_DIR  ${ROOT_DIR}/submodules/tinyddsloader)
set(SHADERC_INCLUDE_DIR        ${ROOT_DIR}/submodules/shaderc/libshaderc/include)

set(MODULES_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}
        #${Vulkan_INCLUDE_DIR}
        ${GLM_INCLUDE_DIR}
        ${GLFW_INCLUDE_DIR}
        ${IMGUI_INCLUDE_DIR}
        ${VMA_INCLUDE_DIR}
        ${GLSLANG_INCLUDE_DIR}
        ${TINYOBJLOADER_INCLUDE_DIR}
        ${SPAN_INCLUDE_DIR}
        ${SPIRV_REFLECT_INCLUDE_DIR}
        ${STB_INCLUDE_DIR}
        ${TINYGLTF_INCLUDE_DIR}
        ${TINYDDSLOADER_INCLUDE_DIR}
        ${SHADERC_INCLUDE_DIR}
)

set(LINK_LIBS
        Vulkan::Vulkan
        glfw
        spirv-cross-core
        spirv-cross-glsl
        glm
        spdlog::spdlog
        shaderc
)

target_include_directories(ENGINE PUBLIC ${MODULES_DIR})
target_link_libraries(ENGINE PUBLIC ${LINK_LIBS})