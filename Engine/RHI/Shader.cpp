#include "Shader.h"



uint16_t Shader::default_vertex_input_binding = 0;

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
        LOG_ERROR("error read"+ path.string());

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
        this->stage_flags = get_shader_flags(kind);

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




void Shader::reflect_shader(const std::vector<uint32_t> binary_code)
{


    spirv_cross::Compiler spirv_compiler(binary_code);

    auto sources = spirv_compiler.get_shader_resources();

    //Uniform buffer

    for (const auto& ub_resouce : sources.uniform_buffers)
    {
        uint32_t set = spirv_compiler.get_decoration(ub_resouce.id, spv::DecorationDescriptorSet);
        uint32_t bind = spirv_compiler.get_decoration(ub_resouce.id, spv::DecorationBinding);
        std::string name = ub_resouce.name;
        ResourceType type = ResourceType::UniformBuffer;



        ResourceLayoutInfo info;
        info.name = name;
        info.type = type;
        info.set = set;
        info.binding = bind;

        resouce_record[set].emplace_back(
            info);

    }


    //To Do :storage_buffer


    for (const auto& storage_buffer : sources.storage_buffers)
    {
        uint32_t set = spirv_compiler.get_decoration(storage_buffer.id, spv::DecorationDescriptorSet);
        uint32_t bind = spirv_compiler.get_decoration(storage_buffer.id, spv::DecorationBinding);
        std::string name = storage_buffer.name;
        ResourceType type = ResourceType::StorageBuffer;



        ResourceLayoutInfo info;
        info.name = name;
        info.type = type;
        info.set = set;
        info.binding = bind;

        resouce_record[set].emplace_back(
            info);
    }


    //To Do: push_constant_buffer
    for (const auto& pushConstant_resource : sources.push_constant_buffers)
    {
        const spirv_cross::SPIRType& type = spirv_compiler.get_type(pushConstant_resource.base_type_id);
       



        if (type.basetype == spirv_cross::SPIRType::Struct)
        {
            uint32_t struct_total_size = 0;

            for (uint32_t i = 0; i < type.member_types.size(); i++)
            {
                uint32_t memberSize = spirv_compiler.get_declared_struct_member_size(type, i);
                struct_total_size += memberSize;


                std::string memberName = spirv_compiler.get_member_name(pushConstant_resource.base_type_id, i);

                // ��ȡ��Ա��ƫ����
                uint32_t memberOffset = spirv_compiler.get_member_decoration(pushConstant_resource.base_type_id, i, spv::DecorationOffset);

                // ��ȡ��Ա������
                const spirv_cross::SPIRType& memberType = spirv_compiler.get_type(type.member_types[i]);
                std::string memberTypeName = spirv_compiler.get_name(type.member_types[i]);


                //todo reflect

            }
           

            //only one push constant
            m_push_constant_specification_collection.Size = struct_total_size;
            m_push_constant_specification_collection.Offset = 0;        
            m_push_constant_specification_collection.Flags = get_shader_flags();

        }

       
    }





    //To Do Sample Image
    for (const auto& sample_image : sources.sampled_images)
    {
        uint32_t set = spirv_compiler.get_decoration(sample_image.id, spv::DecorationDescriptorSet);
        uint32_t bind = spirv_compiler.get_decoration(sample_image.id, spv::DecorationBinding);
        std::string name = sample_image.name;
        ResourceType type = ResourceType::SampledImage;



        ResourceLayoutInfo info;
        info.name = name;
        info.type = type;
        info.set = set;
        info.binding = bind;

        resouce_record[set].emplace_back(
            info);
    }



    //acceleration_structures 
    for (const auto& as : sources.acceleration_structures)
    {
        uint32_t set = spirv_compiler.get_decoration(as.id, spv::DecorationDescriptorSet);
        uint32_t bind = spirv_compiler.get_decoration(as.id, spv::DecorationBinding);
        std::string name = as.name;
        ResourceType type = ResourceType::AccelerationStructure;



        ResourceLayoutInfo info;
        info.name = name;
        info.type = type;
        info.set = set;
        info.binding = bind;

        resouce_record[set].emplace_back(
            info);
    }


    //Input attachments for vertex shader


    //VkVertexInputAttributeDescription -> VkVertexInputBindingDescription then pass to pipeline VkPipelineVertexInputStateCreateInfo
    if (this->stage_flags == VK_SHADER_STAGE_VERTEX_BIT)
    {
        isVertex = true;
        uint32_t offset = 0;
        for (const auto& resource : sources.stage_inputs)
        {
            auto attachment_idx = spirv_compiler.get_decoration(resource.id, spv::DecorationLocation);

            auto type = spirv_compiler.get_type(resource.type_id);
            auto base_type = spirv_compiler.get_type(resource.base_type_id);
            auto vec_size = type.vecsize;


            VkFormat format;
            if (base_type.basetype == spirv_cross::SPIRType::Float)
            {
                if (vec_size == 1) format = VK_FORMAT_R32_SFLOAT;
                else if (vec_size == 2) format = VK_FORMAT_R32G32_SFLOAT;
                else if (vec_size == 3) format = VK_FORMAT_R32G32B32_SFLOAT;
                else if (vec_size == 4) format = VK_FORMAT_R32G32B32A32_SFLOAT;
            }

            VkVertexInputAttributeDescription attributeDescription{};

            attributeDescription.location = attachment_idx;
            attributeDescription.binding = default_vertex_input_binding;
            attributeDescription.format = format;
            attributeDescription.offset = offset;


            vertex_input_attributs.push_back(attributeDescription);
            offset += vec_size * sizeof(float);


        }
        

        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.stride = offset;
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        bindingDescription.binding = default_vertex_input_binding;

        // VkPipelineVertexInputStateCreateInfo

        vertex_input_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertex_input_state.vertexBindingDescriptionCount = 1;
        vertex_input_state.vertexAttributeDescriptionCount = vertex_input_attributs.size();
        vertex_input_state.pVertexAttributeDescriptions = vertex_input_attributs.data();
        vertex_input_state.pVertexBindingDescriptions = &bindingDescription;
    }


}

void Shader::PrintResourceRecord(const std::unordered_map<uint32_t, std::vector<ResourceLayoutInfo>>& resource_record)
{
    for (const auto& pair : resource_record) {
        const auto& key = pair.first;
        const auto& resources = pair.second;

        LOG_INFO("Key: " ,key);

        for (const auto& resource : resources) {
            LOG_INFO(resource);
        }
    }
}

std::string operator<<(std::ostream& os, const ResourceLayoutInfo& info)
{
    
    std::ostringstream ss;
    ss << "Name: " << info.name
        << ", Type: " << ResourceTypeToString(info.type)
        << ", Set: " << info.set
        << ", Binding: " << info.binding;
    return ss.str();
}
