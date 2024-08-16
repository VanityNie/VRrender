#include"pch.h"
#include"Shader.h"


class Shader;


struct Setting
{

	std::vector<Shader> shaders;
	

};


class Pipeline
{

private:
	VkPipeline handle;
	VkPipelineLayout pipeline_layout;
	VkPipelineCache pipeline_cache;

	

public:
	Pipeline(const Setting & setting)
	{

	}


};