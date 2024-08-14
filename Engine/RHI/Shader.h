


#include"pch.h"
#include"Device.h"



struct DescriptorMeta {


};


class ShaderFactory
{

	
public:
	
	
};

class Shader
{

private:
	VkShaderModule shader_module;
	VkShaderStageFlags stage_flags;
	std::string name;
	std::vector<uint32_t> spirv_code;


	std::string extract_filename(const std::string& path) {
		std::filesystem::path fs_path(path);
		// 使用 filesystem::path 的 filename 方法获取文件名
		return fs_path.filename().string();
	}
	VkDevice m_device;

public:

	
	
	Shader(std::string_view file_name);

	auto get_spirv_code() { return spirv_code; }
	VkShaderModule create_vk_shader_module(const VkDevice& device) const;

	
};