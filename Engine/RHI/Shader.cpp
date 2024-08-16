#include "Shader.h"

std::vector<uint32_t> Shader::read_spvdata(std::string_view file_name)
{
    std::filesystem::path path(file_name);

    
    if (!std::filesystem::exists(path)) {
        // �ļ������ڣ����ؿ�����
        return std::vector<uint32_t>();
    }

    std::ifstream file(path, std::ios::binary);
    if (!file) {
        LOG_ERROR(file_name);
        return std::vector<uint32_t>();
    }

    // ��ȡ�ļ���С
    size_t fileSize = std::filesystem::file_size(path);

    // ������Ҫ���ٸ�uint32_t���洢�ļ�����
    size_t numWords = fileSize / sizeof(uint32_t);

    // ��ȡ���ݵ�����
    std::vector<uint32_t> data(numWords);
    if (file.read(reinterpret_cast<char*>(data.data()), fileSize)) {
        // �ɹ���ȡ
        return data;
    }
    else {
        // ��ȡʧ�ܣ����ؿ�����
        return std::vector<uint32_t>();
    }
}

std::vector<uint32_t> Shader::complie_shader(std::string_view file_name, shaderc_shader_kind kind, const shaderc::CompileOptions& compile_option)
{

    shaderc::Compiler compiler;
   
    
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


Shader::Shader(std::string_view file_name, shaderc::CompileOptions compile_option)
{
	std::filesystem::path fs_path(file_name);
	auto extension = fs_path.extension().string();
	this->name = extract_filename(std::string( file_name ));
    


	if (extension == "spv")
	{
		//read spv
		this->spirv_code = read_spvdata(file_name);
	}

    //compile glsl data
    else {

        auto kind = get_shader_kind(extension);
        this->spirv_code = complie_shader(fs_path.string(), kind, compile_option);
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

shaderc_shader_kind Shader::get_shader_kind(const std::string& path)
{

   return mstages[path];
}



