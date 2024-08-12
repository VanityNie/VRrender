#include"pch.h"
#include"Device.h"




class RenderGraph;

class AppBase
{


private:
	Device m_device;
	VulkanContext m_context;

	std::vector<VkFence> in_flight_fences;
	std::vector<VkFence> images_in_flight;

	

public:

	std::unique_ptr<RenderGraph> rg;

	void prepareFrame();

	void submitFrame();



};