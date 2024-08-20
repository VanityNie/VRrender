



#include"RHI/pch.h"




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






}









