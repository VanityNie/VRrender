//
// Created by win111 on 2024/7/4.
//

#ifndef VULKANABSTRACTIONLAYER_DEVICE_H
#define VULKANABSTRACTIONLAYER_DEVICE_H

#include "vulkan/vulkan.h"
#include "VulkanContext.h"
#include"vma/vk_mem_alloc.h"



class DeviceFeatureManager {
private:
    VkPhysicalDeviceFeatures2 device_features2{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2 };
    VkPhysicalDeviceVulkan12Features features12{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES };
    VkPhysicalDeviceVulkan13Features features13{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES };
    VkPhysicalDeviceRayTracingPipelineFeaturesKHR rt_fts{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR };
    VkPhysicalDeviceAccelerationStructureFeaturesKHR accel_fts{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR };
    VkPhysicalDeviceShaderAtomicFloatFeaturesEXT atomic_fts{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_FLOAT_FEATURES_EXT };
    VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamic_rendering_feature{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR };
    VkPhysicalDeviceSynchronization2FeaturesKHR syncronization2_features{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES_KHR };
    VkPhysicalDeviceMaintenance4FeaturesKHR maintenance4_fts{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_4_FEATURES_KHR };

    std::vector<void*> feature_chain;

    void setup_features(bool use_dynamic_rendering) {
        // 设置原子特性
        atomic_fts.shaderBufferFloat32AtomicAdd = VK_TRUE;
        atomic_fts.shaderBufferFloat32Atomics = VK_TRUE;
        atomic_fts.shaderSharedFloat32AtomicAdd = VK_TRUE;
        atomic_fts.shaderSharedFloat32Atomics = VK_TRUE;

        // 设置加速结构特性
        accel_fts.accelerationStructure = VK_TRUE;

        // 设置光线追踪特性
        rt_fts.rayTracingPipeline = VK_TRUE;

        // 设置Vulkan 1.2特性
        features12.bufferDeviceAddress = VK_TRUE;
        features12.runtimeDescriptorArray = VK_TRUE;
        features12.shaderSampledImageArrayNonUniformIndexing = VK_TRUE;
        features12.scalarBlockLayout = VK_TRUE;

        // 设置设备基本特性
        device_features2.features.samplerAnisotropy = VK_TRUE;
        device_features2.features.shaderInt64 = VK_TRUE;
        device_features2.features.fragmentStoresAndAtomics = VK_TRUE;
        device_features2.features.vertexPipelineStoresAndAtomics = VK_TRUE;

        // 添加特性到链中
        feature_chain = { &features12, &rt_fts, &accel_fts, &atomic_fts };

        if (use_dynamic_rendering) {
            dynamic_rendering_feature.dynamicRendering = VK_TRUE;
            syncronization2_features.synchronization2 = VK_TRUE;
            maintenance4_fts.maintenance4 = VK_TRUE;

            feature_chain.insert(feature_chain.begin(), { &maintenance4_fts, &syncronization2_features, &dynamic_rendering_feature });
        }
        else {
            feature_chain.insert(feature_chain.begin(), &features13);
        }

        link_feature_chain();
    }

    void link_feature_chain() {
        if (feature_chain.empty()) {
            device_features2.pNext = nullptr;
            return;
        }

        device_features2.pNext = feature_chain[0];
        for (size_t i = 0; i < feature_chain.size() - 1; i++) {
            *reinterpret_cast<void**>(feature_chain[i]) = feature_chain[i + 1];
        }
        *reinterpret_cast<void**>(feature_chain.back()) = nullptr;
    }

public:
    DeviceFeatureManager(bool use_dynamic_rendering = true) {
        setup_features(use_dynamic_rendering);
    }

    template<typename FeatureType>
    void add_feature(FeatureType& feature) {
        feature_chain.push_back(&feature);
        link_feature_chain();
    }

    template<typename FeatureType>
    FeatureType* get_feature() {
        for (void* ptr : feature_chain) {
            if (reinterpret_cast<VkBaseOutStructure*>(ptr)->sType == FeatureType::structureType) {
                return reinterpret_cast<FeatureType*>(ptr);
            }
        }
        return nullptr;
    }

    void enable_feature(VkBool32 VkPhysicalDeviceFeatures::* feature) {
        *(&(device_features2.features.*feature)) = VK_TRUE;
    }

    void* get_feature_chain() {
        return &device_features2;
    }

   
};

enum class QueueType { GFX, COMPUTE, PRESENT };

class Device {


private:
    VkDevice device;
   
    
    std::shared_ptr<VulkanContext>m_context;
    
    std::vector<const char*> device_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    void* device_create_chain;
 

    std::vector<VkQueue> queues;



public:
    explicit Device(VulkanContext* context);


    VkQueue get_graphic_queue() { return queues[(int)QueueType::GFX]; }
    VkQueue get_compute_queue() { return queues[(int)QueueType::COMPUTE]; }
    VkQueue get_present_queue() { return queues[(int)QueueType::PRESENT]; }
    DeviceFeatureManager device_feature_manager;
    void create_device();
    VkDevice get_device() { return device; }
    std::shared_ptr<VulkanContext> get_vulkan_context() { return m_context; }
    VmaAllocator m_memory_alloactor;
};


#endif //VULKANABSTRACTIONLAYER_DEVICE_H
