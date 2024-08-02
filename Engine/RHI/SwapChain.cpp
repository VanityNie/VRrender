#include "SwapChain.h"

VkResult van::SwapChain::wait_idle()
{
	if (m_waitQueue)
		return vkQueueWaitIdle(m_waitQueue);
	else
		return vkDeviceWaitIdle(m_device->get_device());

}

van::SwapChain::SwapChain(Device* device):m_device(device)
{

	auto  context = this->m_device->get_vulkan_context();

	this->m_phycial_device = context->get_physcial_device();
	this->m_surface = context->get_swapchain_coontext().surface;
	

	m_queue = device->get_graphic_queue();
	m_queueFamilyIndex = context->get_queue_indices().gfx_family.value();


}

van::SwapChain::SwapChain(VkDevice device, VkPhysicalDevice physicalDevice, VkQueue queue, uint32_t queueFamilyIndex,
	VkSurfaceKHR surface, VkFormat format, VkImageUsageFlags imageUsage)
{

	init(device, physicalDevice, queue, queueFamilyIndex, surface, format, imageUsage);
}

void van::SwapChain::deinit()
{
}

VkExtent2D van::SwapChain::update(int width, int height, bool vsync)
{
	m_changeID++;

	check(wait_idle(), "wait Idle error");

	auto surface_capabilities = m_device->get_vulkan_context()->get_swapchain_coontext().
		swap_chian_support_details.capabilities;
	std::vector<VkPresentModeKHR> present_modes = m_device->get_vulkan_context()->get_swapchain_coontext().
		swap_chian_support_details.presentModes;


	VkExtent2D swapchainExtent;



	if (surface_capabilities.currentExtent.width == (uint32_t)-1)
	{
		// If the surface size is undefined, the size is set to
		// the size of the images requested.
		swapchainExtent.width = width;
		swapchainExtent.height = height;
	}
	
	else
	{
		// If the surface size is defined, the swap chain size must match
		swapchainExtent = surface_capabilities.currentExtent;
	}

	assert(swapchainExtent.width && swapchainExtent.height);
	int presentModecount = present_modes.size();
	// everyone must support FIFO mode
	VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
	if (!vsync)
	{
		for (uint32_t i = 0; i < presentModecount; i++)
		{
			if (present_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
			}
			if (present_modes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)
			{
				swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
			}
			if (swapchainPresentMode == m_preferredVsyncOffMode)
			{
				break;
			}
		}
	}
	// Determine the number of VkImage's to use in the swap chain (we desire to
  // own only 1 image at a time, besides the images being displayed and
  // queued for display):
	uint32_t desiredNumberOfSwapchainImages = surface_capabilities.minImageCount + 1;
	if ((surface_capabilities.maxImageCount > 0) && (desiredNumberOfSwapchainImages > surface_capabilities.maxImageCount))
	{
		// Application must settle for fewer images than desired:
		desiredNumberOfSwapchainImages = surface_capabilities.maxImageCount;
	}

	VkSurfaceTransformFlagBitsKHR preTransform;

	if (surface_capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
	{
		preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	}
	else
	{
		preTransform = surface_capabilities.currentTransform;
	}

	//update
}



bool van::SwapChain::init(VkDevice device, VkPhysicalDevice physicalDevice, VkQueue queue, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkFormat format, VkImageUsageFlags imageUsage)
{

	auto  context = this->m_device->get_vulkan_context();

	return false;
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
	swapChainInfo.imageFormat = m_format;

}
