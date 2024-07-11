#pragma once


#define  VK_NO_PROTOTYPES
#include "vulkan/vulkan.h"
#include <GLFW/glfw3.h>
#include <optional>
#include<vector>

// Utils
struct QueueFamilyIndices {
	std::optional<uint32_t> gfx_family;
	std::optional<uint32_t> present_family;
	std::optional<uint32_t> compute_family;

	// TODO: Extend to other families
	bool is_complete() { return (gfx_family.has_value() && present_family.has_value()) && compute_family.has_value(); }
};

struct DescriptorInfo {
	union {
		VkDescriptorImageInfo image;
		VkDescriptorBufferInfo buffer;
	};
	DescriptorInfo() {}
	DescriptorInfo(const VkDescriptorImageInfo& image) { this->image = image; }

	DescriptorInfo(const VkDescriptorBufferInfo& buffer) { this->buffer = buffer; }
};



enum class QueueType { GFX, COMPUTE, PRESENT };



namespace vk {

    enum class RenderStage {SATGE_VERTEX, STAGE_TCS,STAGE_TES,STAGE_GEO, STAGE_FRAGMENT};
//to do
    enum class Component{POSTION, NORMAL, COLOR, UV, TANGET };


}


