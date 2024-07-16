//
// Created by win111 on 2024/7/9.
//
#include "VulkanContext.h"

#include "../../submodules/glfw/src/null_joystick.h"


VulkanContext::VulkanContext(Windows *windows) {

    this->window_ptr = windows->getWindowHandle();



}


void VulkanContext::create_instance() {
    VkApplicationInfo app_info{};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "VkRender";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 1);
    app_info.pEngineName = "Vanity";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 1);
    app_info.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo instance_CI{};
    instance_CI.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_CI.pApplicationInfo = &app_info;

    auto extensions = get_req_instance_extensions();
    instance_CI.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    instance_CI.ppEnabledExtensionNames = extensions.data();

    //debug validation layers
    if (enable_validation_layers) {
        instance_CI.enabledLayerCount = static_cast<uint32_t>(validation_layers_lst.size());
        instance_CI.ppEnabledLayerNames = validation_layers_lst.data();

        VkDebugUtilsMessengerCreateInfoEXT debug_CI {};
        debug_CI.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debug_CI.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
        debug_CI.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debug_CI.pfnUserCallback = &VulkanContext::debug_callback;

        instance_CI.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debug_CI;

        //VkDebugUtilsMessengerCreateInfoEXT debug_CI = vk::debug_messenger_CI(debug_callback);
        //instance_CI.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debug_CI;


    } else {
        instance_CI.enabledLayerCount = 0;
        instance_CI.pNext = nullptr;
    }

    //vkCreateInstance(&instance_CI,nullptr,&instance);
    check(vkCreateInstance(&instance_CI,nullptr,&instance),"Failed to crate Instance");

    if(enable_validation_layers && !check_validation_layer_support()) {
        LOG_ERROR("Validation layers requested, but not available!");
    }
}

/*
 * @return instance extensions: GLFW and DEBUG extension
 */

std::vector<const char *> VulkanContext::get_req_instance_extensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enable_validation_layers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    return extensions;
}

VkResult
VulkanContext::vkExt_create_debug_messenger(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                            const VkAllocationCallbacks *pAllocator,
                                            VkDebugUtilsMessengerEXT *pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

/*
 * clean vkDestroyDebugUtisMessengerEXT
 */
void VulkanContext::vkExt_destroy_debug_messenger(VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger,
                                                  const VkAllocationCallbacks *pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debug_messenger, pAllocator);
    }
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                     VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                     const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                     void* pUserData) {
    // if ((messageSeverity & (VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
    //	VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)) ==
    //	0) {
    //	return VK_TRUE;
    // }

    if ((messageSeverity & (VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)) == 0) {
        // LUMEN_TRACE("Validation Warning: {0} ", pCallbackData->pMessage);
        return VK_TRUE;
    }
    LOG_ERROR("Validation Error: {0} ", pCallbackData->pMessage);
    return VK_FALSE;
}


VkBool32 VulkanContext::debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) {
    if ((messageSeverity & (VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)) == 0) {
        // LUMEN_TRACE("Validation Warning: {0} ", pCallbackData->pMessage);
        return VK_TRUE;
    }
    LOG_ERROR("Validation Error: {0} ", pCallbackData->pMessage);
    return VK_FALSE;

}

void VulkanContext::create_surface() {

    check(
        glfwCreateWindowSurface(this->instance,window_ptr,nullptr,&swap_chian_context.surface),
        "Failed to create window surface");

}


void VulkanContext::pick_up_physical_device() {
    uint32_t device_cnt = 0;
    vkEnumeratePhysicalDevices(instance,&device_cnt,nullptr);
    if(device_cnt==0) {
        LOG_ERROR("Failed to find GPUs with Vulkan support");
    }

    std::vector<VkPhysicalDevice> devices(device_cnt);
    vkEnumeratePhysicalDevices(instance, &device_cnt, devices.data());




}

void VulkanContext::clean_up() {

    if(enable_validation_layers) {
        vkExt_destroy_debug_messenger(instance,debug_messenger,nullptr);
    }
    vkDestroyInstance(instance,nullptr);

}

QueueFamilyIndices VulkanContext::find_queue_familyes() {
    QueueFamilyIndices indices;
    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count,
                                             nullptr);
    queue_famliy.resize(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count,
                                             queue_famliy.data());
    int i = 0;

    for (const auto &queueFamily: queue_famliy) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.gfx_family = i;
        }
        if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
            indices.compute_family = i;
        }

        VkBool32 present_support = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, swap_chian_context.surface, &present_support);

        if (present_support) {
            indices.present_family = i;
        }
        if (indices.is_complete()) {
            break;
        }
        i++;
    }
    return indices;
}

SwapChianSupportDetails VulkanContext::query_swap_chain_support() {
    SwapChianSupportDetails details;


    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device,swap_chian_context.surface,
                                              &details.capabilities);

    //get format support info
    uint32_t format_cnt;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device,swap_chian_context.surface,
                                         &format_cnt, nullptr);

    if(format_cnt!=0)
    {
        details.formats.resize(format_cnt);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device,swap_chian_context.surface,
                                             &format_cnt, details.formats.data());
    }

    uint32_t present_mode_cnt;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, swap_chian_context.surface, &present_mode_cnt, nullptr);

    //get present mode support
    if(present_mode_cnt !=0)
    {
        details.presentModes.resize(present_mode_cnt);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device,swap_chian_context.surface,
                                                  &present_mode_cnt,details.presentModes.data());
    }

    return details;
}

bool VulkanContext::is_suitable_physical_device() {
    QueueFamilyIndices familyIndices = find_queue_familyes();



    //physical device extension support

    auto extension_supported = [this]()
    {
        uint32_t extension_cnt;
        vkEnumerateDeviceExtensionProperties(physical_device, nullptr,
                                             &extension_cnt, nullptr);

        std::vector<VkExtensionProperties> available_extensions(extension_cnt);
        vkEnumerateDeviceExtensionProperties(physical_device, nullptr,
                                             &extension_cnt, available_extensions.data());
        std::set<std::string> required_extensions(physical_device_extensions.begin(),physical_device_extensions.end());

        for (const auto& extension : available_extensions) {
            required_extensions.erase(extension.extensionName);
        }

        return required_extensions.empty();
    };

    bool swapchain_adequate = false;
    if(extension_supported())
    {
        swap_chian_context.swap_chian_support_details = query_swap_chain_support();
        auto swap_chain_details =  swap_chian_context.swap_chian_support_details;
        swapchain_adequate =  !swap_chain_details.presentModes.empty() && !swap_chain_details.formats.empty();
    }

    return indices.is_complete() && swapchain_adequate;

}






