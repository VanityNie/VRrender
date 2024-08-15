#include "Shader.h"

std::vector<uint32_t> Shader::read_spvdata(std::string_view file_name)
{
    std::filesystem::path path(file_name);

    
    if (!std::filesystem::exists(path)) {
        // 文件不存在，返回空向量
        return std::vector<uint32_t>();
    }

    std::ifstream file(path, std::ios::binary);
    if (!file) {
        LOG_ERROR(file_name);
        return std::vector<uint32_t>();
    }

    // 获取文件大小
    size_t fileSize = std::filesystem::file_size(path);

    // 计算需要多少个uint32_t来存储文件内容
    size_t numWords = fileSize / sizeof(uint32_t);

    // 读取数据到向量
    std::vector<uint32_t> data(numWords);
    if (file.read(reinterpret_cast<char*>(data.data()), fileSize)) {
        // 成功读取
        return data;
    }
    else {
        // 读取失败，返回空向量
        return std::vector<uint32_t>();
    }
}

std::vector<uint32_t> Shader::complie_shader(std::string_view file_name, shaderc_shader_kind kind)
{

    shaderc::Compiler compiler;
    shaderc::CompileOptions compile_option;
    compile_option.SetVulkanRulesRelaxed(true);
    compile_option.SetTargetSpirv(shaderc_spirv_version_1_6);
    
    auto shader_file = reader.ReadAsBinary(file_name);
    
    //todo read file convert to std::string
    std::string shader_str;

    shaderc::SpvCompilationResult module =  compiler.CompileGlslToSpv
    (shader_str, kind, name.c_str(), compile_option);

    if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
    
        LOG_ERROR(module.GetErrorMessage());
        return std::vector<uint32_t>();
    }

    return { module.cbegin(), module.cend() };

	
}


Shader::Shader(std::string_view file_name)
{
	std::filesystem::path fs_path(file_name);
	auto extension = fs_path.extension().string();
	this->name = extract_filename(std::string( file_name ));
    


	if (extension == "spv")
	{
		//read spv
		this->spirv_code = read_spvdata(file_name);
	}
	else
	{
		//complie to spv
		this->spirv_code = complie_shader(file_name);
	}


}

VkShaderModule Shader::create_vk_shader_module(const VkDevice& device) const
{

	VkShaderModuleCreateInfo createInfo;
	VkShaderModule module{};
	createInfo.codeSize = this->spirv_code.size()*sizeof(uint32_t);
	createInfo.flags = this->stage_flags;
	createInfo.pCode = spirv_code.data();
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;



	vkCreateShaderModule(device, &createInfo, nullptr, &module  );


	return module;
}
