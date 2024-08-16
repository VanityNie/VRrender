


#include"pch.h"
#include"Device.h"
#include"ShaderReader.h"

static std::unordered_map<std::string, shaderc_shader_kind> mstages = {
	{"vert", shaderc_vertex_shader}, {"frag", shaderc_fragment_shader}, {"comp", shaderc_compute_shader},
	{"rgen", shaderc_raygen_shader}, {"rahit", shaderc_anyhit_shader},	{"rchit", shaderc_closesthit_shader},
	{"rmiss", shaderc_miss_shader},
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

	std::vector<uint32_t> read_spvdata(std::string_view file_name);

	std::vector<uint32_t> complie_shader(std::string_view file_name, shaderc_shader_kind kind, const shaderc::CompileOptions& compile_option);

public:


	//shaderc::CompileOptions compile_option;
	ShaderReader reader{};
	Shader(std::string_view file_name, shaderc::CompileOptions compile_option);

	auto get_spirv_code() { return spirv_code; }
	VkShaderModule create_vk_shader_module(const VkDevice& device) const;
	shaderc_shader_kind get_shader_kind(const std::string& path);
	

	
};