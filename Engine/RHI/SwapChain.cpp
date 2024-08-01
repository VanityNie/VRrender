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

void van::SwapChain::create_swap_chain()
{

	VkSharingMode share_mode;
	uint32_t queue_famliy_index_count;

	if (m_device->get_vulkan_context()->is_same_Graphic_PresentQueueFamily())
	{
		share_mode = VK_SHARING_MODE_EXCLUSIVE;
		queue_famliy_index_count = 0;
	}




	VkSwapchainCreateInfoKHR swapChainInfo = {
	};

	swapChainInfo.imageSharingMode = share_mode;
}
