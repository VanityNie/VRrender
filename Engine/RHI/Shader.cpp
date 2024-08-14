#include "Shader.h"

Shader::Shader(std::string_view file_name)
{

	this->name = extract_filename(std::string( file_name ));

}

VkShaderModule Shader::create_vk_shader_module(const VkDevice& device) const
{
	return VkShaderModule();
}
