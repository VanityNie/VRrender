



#include"RHI/pch.h"

#include<vulkan/vulkan.h>


namespace tools
{


	VkDescriptorSetLayoutBinding create_desciptorset_layout_binding(VkDescriptorType type, uint32_t binding, VkShaderStageFlags flags, uint32_t descriptor_counts = 1)
	{

		VkDescriptorSetLayoutBinding setLayoutBindings;
		setLayoutBindings.descriptorType = type;
		// Shader binding point
		setLayoutBindings.binding = binding;
		// Accessible from the vertex shader only (flags can be combined to make it accessible to multiple shader stages)
		setLayoutBindings.stageFlags = flags;
		// Binding contains one element (can be used for array bindings)
		setLayoutBindings.descriptorCount = descriptor_counts;

		return setLayoutBindings;
	}




	template <class T>
	constexpr T align_up(T x, size_t a) noexcept {
		return T((x + (T(a) - 1)) & ~T(a - 1));
	}


	inline uint32_t calc_mip_levels(VkExtent2D extent) {
		return static_cast<uint32_t>(std::floor(std::log2(std::max(extent.width, extent.height)))) + 1;
	}


}



namespace DebugMarker
{


#if _DEBUG
	VkDebugUtilsObjectNameInfoEXT debug_utils_name{ VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT, nullptr, type,
												   obj, name };
	
	
#endif
}





