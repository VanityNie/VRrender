#include "Pipeline.h"






VkPipelineBindPoint Pipeline::get_bind_point(PipelineType& type)
{
	switch (type) {
	case PipelineType::GFX: {
		return VK_PIPELINE_BIND_POINT_GRAPHICS;
	}
	case PipelineType::RT: {
		return VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR;
	}
	case PipelineType::COMPUTE: {
		return VK_PIPELINE_BIND_POINT_COMPUTE;
	}
	}
	return VK_PIPELINE_BIND_POINT_MAX_ENUM;
}







void GfxPipeline::create_set_layout(const std::vector<Shader>& shaders)
{
	
	
	for (const auto& shader : shaders)
	{
		
		std::unordered_map< uint32_t, std::vector<ResourceLayoutInfo> > data = shader.get_setlayouts_map();

		VkShaderStageFlagBits  shader_stage = shader.get_shader_flags();

		//descriptor set : rescoureInfo[......]
		for (auto [set, resources] : data)
		{

			//add resource binding
			for (auto resource : resources)
			{

				//get the descriptor layout num by the set
				descriptorset_nums = std::max(set, descriptorset_nums);
				//if has registered , only | shader stage flags 
				if (resource_binding_map.find(resource.name) != resource_binding_map.end())
				{

					auto binding = resource_binding_map[resource.name];
					binding.stageFlags |= shader.get_shader_flags();


				}
				else {

					auto type = resource.type;
					auto binding = resource.binding;
					auto shaderFlags = shader.get_shader_flags();
					auto descirptor_set_binding =
						tools::create_desciptorset_layout_binding(resourceTypeToVkDescriptorType(type), binding, shaderFlags);
					resource_binding_map[resource.name] = descirptor_set_binding;
				}
			}

		
		}
		descriptorset_nums += 1;
	}

	
	get_descriptor_bindings(shaders);



	//descriptor layout 
	for (auto [set, bindings] : set_binding_map)
	{

		VkDescriptorSetLayoutCreateInfo createInfo =   tools::descriptorset_layout_create_info(bindings);

		VkDescriptorSetLayout layout{};
		
		vkCreateDescriptorSetLayout(m_device->get_device(), &createInfo, nullptr, &layout);

		descriptorset_layouts.push_back(layout);
		
	}



	//push constant 
	uint32_t idx = 0;
	for (const auto& shader : shaders)
	{

		auto push_constant = shader.get_pushconstant_collctions();
		VkPushConstantRange range{};
		range.stageFlags = push_constant.Flags;
		range.offset = push_constant.Offset;
		range.size = push_constant.Size;

		push_constant_ranges.emplace_back(range);
	}

}

void GfxPipeline::create_pipeline_layout(const std::vector<Shader>& shaders)
{

	//get_descriptor_bindings(shaders);



	VkPipelineLayoutCreateInfo pipeline_layout_create_info{};

	//Todo finish constant ranges
	//pipeline_layout_create_info.pPushConstantRanges 
	
	pipeline_layout_create_info.pPushConstantRanges = push_constant_ranges.data();
	pipeline_layout_create_info.pushConstantRangeCount = push_constant_ranges.size();
	pipeline_layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

	pipeline_layout_create_info.setLayoutCount = descriptorset_layouts.size();
	pipeline_layout_create_info.pSetLayouts = descriptorset_layouts.data();
	vkCreatePipelineLayout(m_device->get_device(), &pipeline_layout_create_info, nullptr, &pipeline_layout);
}


//get descriptor set bindings to set_binding_map
void GfxPipeline::get_descriptor_bindings(const std::vector<Shader>& shaders)
{
	for (const auto& shader : shaders)
	{
		auto data = shader.get_setlayouts_map();

		for (auto [set, resources] : data)
		{

			for (auto resource : resources) {
				set_binding_map[set].push_back(resource_binding_map[resource.name]);
			}
		}
	}
}

void GfxPipeline::create_update_template(const std::vector<Shader>& shaders)
{
	std::vector<VkDescriptorUpdateTemplateEntry> entries;
	VkDescriptorUpdateTemplateCreateInfo template_create_info = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_UPDATE_TEMPLATE_CREATE_INFO };



	for (auto [a, b] : resource_binding_map)
	{

	}

	
}

