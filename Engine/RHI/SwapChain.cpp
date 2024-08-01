#include "SwapChain.h"

van::SwapChain::SwapChain(Device* device):m_device(device)
{


	this->m_phycial_device = m_device->get_vulkan_context()->get_physcial_device();

	
}

void van::SwapChain::deinit()
{
}

VkExtent2D van::SwapChain::update(int width, int height, bool vsync)
{
	return VkExtent2D();
}
