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
    VkPhysicalDeviceProperties device_properties;
    VkPhysicalDeviceProperties2 device_properties2;
    VkPhysicalDeviceMemoryProperties memory_properties;

    VkPhysicalDeviceRayTracingPipelinePropertiesKHR rt_props{
            VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR};
};

class VulkanContext
{
private:
    GLFWwindow* window_ptr = nullptr;
    VkInstance instance;

    VkPhysicalDevice physical_device = VK_NULL_HANDLE;
    PhyscialDeviceContext physcial_device_context;

    bool enable_validation_layers;
    const std::vector<const char*> validation_layers_lst = {"VK_LAYER_KHRONOS_validation"};
    VkDebugUtilsMessengerEXT debug_messenger;
    bool check_validation_layer_support();

    SwapChianContext swap_chian_context;


    //debug
    VkResult vkExt_create_debug_messenger(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                          const VkAllocationCallbacks* pAllocator,
                                          VkDebugUtilsMessengerEXT* pDebugMessenger);

    void vkExt_destroy_debug_messenger(VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger,
                                       const VkAllocationCallbacks* pAllocator);


    //queue
    std::vector<VkQueue> queues;
    QueueFamilyIndices indices;

    void clean_up();


public:
    VulkanContext(Windows* windows);

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


#endif //VULKANABSTRACTIONLAYER_VULKANCONTEXT_H
