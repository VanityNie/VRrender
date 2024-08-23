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


	}


}

void GfxPipeline::create_pipeline_layout(const std::vector<Shader>& shaders)
{
}

void GfxPipeline::create_update_template(const std::vector<Shader>& shaders)
{
}

