//
// Created by win111 on 2024/4/29.
//

#ifndef VULKANABSTRACTIONLAYER_VULKANCONTEXT_H
#define VULKANABSTRACTIONLAYER_VULKANCONTEXT_H

#include "pch.h"


struct QueueFamilyIndices
{
	std::optional<uint32_t> gfx_family;
	std::optional<uint32_t> present_family;
	std::optional<uint32_t> compute_family;

	//more family in future

	bool is_complete()
	{ return (gfx_family.has_value() && present_family.has_value()) && compute_family.has_value(); }

};

struct DescriptorInfo
{

    static void print(){
        LOG_INFO("Descriptor:");
        LOG_TRACE("Trace info:");

    }
};

struct SwapChianSupportDetails {

    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct SwapChianContext {
    //swapChain
    VkExtent2D swapchain_extent;
    VkSwapchainKHR swapchain;
    VkSurfaceKHR surface;

    SwapChianSupportDetails swap_chian_support_details;
};

struct PhyscialDeviceContext
{
    VkPhysicalDeviceFeatures supported_features;
    VkPhysicalDeviceFeatures2 supported_features2;
    VkPhysicalDeviceProperties device_properties;
    VkPhysicalDeviceProperties2 device_properties2;
    VkPhysicalDeviceMemoryProperties memory_properties;

    VkPhysicalDeviceRayTracingPipelinePropertiesKHR rt_props{
            VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR};

     void print_info() {
        LOG_INFO("Vulkan Physical Device Information:");
         LOG_INFO("  Device Name: {}",device_properties.deviceName);

        LOG_INFO("  Memory Heaps: {}", memory_properties.memoryHeapCount);
        for (uint32_t i = 0; i < memory_properties.memoryHeapCount; ++i) {
            LOG_INFO("    Heap {}: {} MB", i, memory_properties.memoryHeaps[i].size / (1024 * 1024));
        }

    }

};

class VulkanContext
{
private:
    GLFWwindow* window_ptr = nullptr;
    VkInstance instance;

    VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;
    PhyscialDeviceContext physcial_device_context;
    std::vector<const char*> physical_device_extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    bool is_suitable_physical_device(VkPhysicalDevice physical_device);

    QueueFamilyIndices find_queue_familyes(VkPhysicalDevice physical_device);

    bool enable_validation_layers;
    const std::vector<const char*> validation_layers_lst = {"VK_LAYER_KHRONOS_validation"};
    VkDebugUtilsMessengerEXT debug_messenger;
    bool check_validation_layer_support();

    SwapChianContext swap_chian_context;
    SwapChianSupportDetails query_swap_chain_support(VkPhysicalDevice physical_device) const;
    void create_surface();

    //debug
    VkResult vkExt_create_debug_messenger(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                          const VkAllocationCallbacks* pAllocator,
                                          VkDebugUtilsMessengerEXT* pDebugMessenger);

    void vkExt_destroy_debug_messenger(VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger,
                                       const VkAllocationCallbacks* pAllocator);


    //queue
    std::vector<VkQueueFamilyProperties> queue_famliy;
    std::vector<VkQueue> queues;
    QueueFamilyIndices indices;

    void clean_up();


public:
    VulkanContext(Windows* windows,bool enableDebug);

    void debug_util_create();

    void create_physical_device();
    void pick_up_physical_device();
    void create_instance();

    VkInstance get_instance(){return this->instance;}

    std::vector<const char*> get_req_instance_extensions();
    static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                         VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                         const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                         void* pUserData);

};



inline void check(VkResult result, const char* msg = 0) {
    if (result != VK_SUCCESS && msg) {
        LOG_ERROR(msg);
    }
}

template <std::size_t Size>
inline void check(std::array<VkResult, Size> results, const char* msg = 0) {
    for (const auto& result : results) {
        if (result != VK_SUCCESS && msg) {
            LOG_ERROR(msg);
        }
    }
}


#endif //VULKANABSTRACTIONLAYER_VULKANCONTEXT_H
