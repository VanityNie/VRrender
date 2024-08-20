


#include"pch.h"
#include"Device.h"
#include"ShaderReader.h"

static std::unordered_map<std::string, shaderc_shader_kind> mstages = {
	{"vert", shaderc_vertex_shader}, {"frag", shaderc_fragment_shader}, {"comp", shaderc_compute_shader},
	{"rgen", shaderc_raygen_shader}, {"rahit", shaderc_anyhit_shader},	{"rchit", shaderc_closesthit_shader},
	{"rmiss", shaderc_miss_shader},
};


static std::unordered_map<shaderc_shader_kind, VkShaderStageFlagBits> shader_stage_maps = {
	{shaderc_vertex_shader, VK_SHADER_STAGE_VERTEX_BIT},
	{shaderc_fragment_shader, VK_SHADER_STAGE_FRAGMENT_BIT},
	{shaderc_compute_shader, VK_SHADER_STAGE_COMPUTE_BIT},
	{shaderc_raygen_shader, VK_SHADER_STAGE_RAYGEN_BIT_KHR},
	{shaderc_anyhit_shader, VK_SHADER_STAGE_ANY_HIT_BIT_KHR},
	{shaderc_closesthit_shader, VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR},
	{shaderc_miss_shader, VK_SHADER_STAGE_MISS_BIT_KHR}
};


enum class ResourceType : uint32_t {
	Sampler = VK_DESCRIPTOR_TYPE_SAMPLER,
	CombinedImageSampler = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
	SampledImage = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
	StorageImage = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
	UniformTexelBuffer = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
	StorageTexelBuffer = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,
	UniformBuffer = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
	StorageBuffer = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
	UniformBufferDynamic = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
	StorageBufferDynamic = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC,
	InputAttachment = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
	InlineUniformBlock = VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK,
	AccelerationStructure = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR,
	SampleWeightImage = VK_DESCRIPTOR_TYPE_SAMPLE_WEIGHT_IMAGE_QCOM,
	BlockMatchImage = VK_DESCRIPTOR_TYPE_BLOCK_MATCH_IMAGE_QCOM,
	Mutable = VK_DESCRIPTOR_TYPE_MUTABLE_EXT
};

VkDescriptorType resourceTypeToVkDescriptorType(ResourceType type) {
	return static_cast<VkDescriptorType>(type);
}

struct PushConstantSpecification
{
	std::string      Name;
	uint32_t         Size;
	uint32_t         Offset;
	VkShaderStageFlagBits Flags;
};


struct ResourceLayoutInfo
{
	std::string name ="null";
	ResourceType type;
	uint32_t set;
	uint32_t binding;

	//VkShaderStageFlagBits flags;
	ResourceLayoutInfo() = default;
	ResourceLayoutInfo(std::string _name,ResourceType _type,uint32_t _set, uint32_t _binding):
		name(_name),type(_type),set(_set),binding(_binding)

	{

	}


};


class Shader
{

private:
	VkShaderModule shader_module;
	VkShaderStageFlagBits stage_flags;
	std::string name;
	std::vector<uint32_t> spirv_code;
	std::string extract_filename(const std::string& path) {
		std::filesystem::path fs_path(path);
		// 使用 filesystem::path 的 filename 方法获取文件名
		return fs_path.filename().string();
	}
	VkDevice m_device;

	std::unordered_map< uint32_t, std::vector<ResourceLayoutInfo> > resouce_record;


	std::vector<uint32_t> read_spvdata(std::string_view file_name);
	std::vector<uint32_t> complie_shader(std::string_view file_name, shaderc_shader_kind kind, const shaderc::CompileOptions& compile_option);



	//only one push constant spec 
	std::vector< PushConstantSpecification>m_push_constant_specification_collection;
	

	void reflect_shader(const std::vector<uint32_t> binary_code);


public:


	//shaderc::CompileOptions compile_option;
	ShaderReader reader{};
	Shader(std::string_view file_name, shaderc::CompileOptions compile_option);

	auto get_spirv_code() { return spirv_code; }
	VkShaderModule create_vk_shader_module(const VkDevice& device) const;
	shaderc_shader_kind get_shader_kind(const std::string& path);
	
	VkShaderStageFlagBits get_shader_flags(const shaderc_shader_kind& kind) {
		return shader_stage_maps[kind];
	}
	VkShaderStageFlagBits get_shader_flags()const {
		return stage_flags;
	}
	std::unordered_map< uint32_t, std::vector<ResourceLayoutInfo> > get_setlayouts_map() const { return this->resouce_record; }
};