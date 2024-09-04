#include"pch.h"
#include"Shader.h"


class Shader;

class RenderPass;






struct dim3 {
	uint32_t x = 1;
	uint32_t y = 1;
	uint32_t z = 1;
};

struct GraphicSettings
{

	std::vector<Shader> shaders;
	uint32_t width;
	uint32_t height;
	VkClearValue clear_color;
	VkClearValue clear_depth_stencil;
	VkCullModeFlags cull_mode = VK_CULL_MODE_FRONT_BIT;
	std::vector<uint32_t> specialization_data = {};
	std::vector<bool> blend_enables = {};
	VkFrontFace front_face = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	VkPolygonMode polygon_mode = VK_POLYGON_MODE_FILL;
	VkSampleCountFlagBits sample_count = VK_SAMPLE_COUNT_1_BIT;
	VkIndexType index_type = VK_INDEX_TYPE_UINT32;
	float line_width = 1.0;

	std::function<void(VkCommandBuffer cmd, const RenderPass& pass)> pass_func;

	//Buffer Texture setting to do


};

struct RTPassSettings {
	std::vector<Shader> shaders;
	
	uint32_t recursion_depth = 1;
	std::vector<uint32_t> specialization_data = {};
	dim3 dims;
	std::function<void(VkCommandBuffer cmd, const RenderPass& pass)> pass_func;
};


struct ComputePassSettings {
	Shader shader;
	
	std::vector<uint32_t> specialization_data = {};
	dim3 dims;
	std::function<void(VkCommandBuffer cmd, const RenderPass& pass)> pass_func;
};

enum class PipelineType { GFX = 0, RT = 1, COMPUTE = 2 };

class Pipeline
{


protected:
	VkPipeline handle = VK_NULL_HANDLE;
	VkPipelineLayout pipeline_layout = VK_NULL_HANDLE;
	VkPipelineCache pipeline_cache = VK_NULL_HANDLE;
	VkDescriptorSetLayout set_layout = VK_NULL_HANDLE;
	PipelineType pipeline_type;

	VkDescriptorUpdateTemplate update_template = nullptr;
	VkShaderStageFlags pc_stages = 0;


	std::string name;


	std::mutex mut;
	std::condition_variable cv;


	size_t get_desc_info_size(VkDescriptorType type) {
		switch (type) {
		case VK_DESCRIPTOR_TYPE_SAMPLER:
		case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
		case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
		case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
			return sizeof(VkDescriptorImageInfo);
		case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
		case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
			return sizeof(VkDescriptorBufferInfo);
		case VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR:
			return sizeof(VkWriteDescriptorSetAccelerationStructureKHR);

		default:
			LOG_ERROR("Unimplemented descriptor type!");
			return (size_t)0;

		};
	}

	VkPipelineBindPoint get_bind_point(PipelineType & type);

	virtual void create_set_layout(const std::vector<Shader>& shaders) = 0;
	virtual void create_pipeline_layout(const std::vector<Shader>& shaders) = 0;
	virtual void create_update_template(const std::vector<Shader>& shaders) = 0;


public:
	Pipeline()
	{

	}


};




class GfxPipeline : public Pipeline
{

private:

	std::shared_ptr<Device> m_device;

	std::vector<VkDescriptorSetLayout> descriptorset_layouts;
	
	std::unordered_map<std::string, VkDescriptorSetLayoutBinding> resource_binding_map;
	std::unordered_map<uint32_t, std::vector<VkDescriptorSetLayoutBinding>> set_binding_map;
	std::vector<VkDescriptorSetLayout> descriptor_set_layouts;
	std::vector<VkPushConstantRange>push_constant_ranges;



	uint32_t descriptorset_nums{ 0 };

public:
	struct {
		VkPipelineShaderStageCreateInfo vert_shader_CI;
		VkPipelineShaderStageCreateInfo frag_shader_CI;
		VkPipelineInputAssemblyStateCreateInfo input_asssembly_CI;
		VkPipelineViewportStateCreateInfo viewport_state;
		VkPipelineRasterizationStateCreateInfo rasterizer;
		VkPipelineMultisampleStateCreateInfo multisampling;
		VkPipelineColorBlendAttachmentState color_blend_attachment;
		VkPipelineColorBlendStateCreateInfo color_blend;
		VkPipelineDynamicStateCreateInfo dynamic_state_CI;
		VkGraphicsPipelineCreateInfo pipeline_CI;
	} gfx_cis;


	GfxPipeline() {
		
		pipeline_type = PipelineType::GFX;

	}
	
	
	//get the descirptor layout binding from shaders
	void create_set_layout(const std::vector<Shader>& shaders) override;

	void create_pipeline_layout(const std::vector<Shader>& shaders) override;
	void get_descriptor_bindings(const std::vector<Shader>& shaders);
	virtual void create_update_template(const std::vector<Shader>& shaders) override;

};






class ComputePipeline : public Pipeline
{

};


class RayTracingPipeline : public Pipeline

{


};