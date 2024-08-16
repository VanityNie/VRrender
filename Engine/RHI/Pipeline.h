#include"pch.h"
#include"Shader.h"


class Shader;


struct Settings
{

	std::vector<Shader> shaders;


};


class Pipeline
{

private:
	VkPipeline handle;
	VkPipelineLayout pipeline_layout;


public:
	Pipeline()
	{

	}


};