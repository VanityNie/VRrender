#include "SwapChain.h"

VkResult van::SwapChain::wait_idle()
{
	if (m_waitQueue)
		return vkQueueWaitIdle(m_waitQueue);
	else
		return vkDeviceWaitIdle(m_device_context->get_device());

}

van::SwapChain::SwapChain(Device* device):m_device_context(device)
{

	auto  context = this->m_device_context->get_vulkan_context();
	
	m_device = device->get_device();
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

void van::SwapChain::update_swpachain_images()
{

	check(vkGetSwapchainImagesKHR(m_device_context->get_device(), m_swapchain, &m_imageCount, nullptr), "Get SwapChain Images error");

	m_entries.resize(m_imageCount);
	m_barriers.resize(m_imageCount);

	std::vector<VkImage> images(m_imageCount);
	check(vkGetSwapchainImagesKHR(m_device_context->get_device(), m_swapchain, &m_imageCount, images.data()), "SwapChain images write error");

	//
	// Image views
	//

	for (uint32_t i = 0; i < m_imageCount; i++)
	{

		Entry& entry = m_entries[i];

		// image
		entry.image = images[i];

		// imageview
		VkImageViewCreateInfo viewCreateInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			nullptr,
			0,
			entry.image,
			VK_IMAGE_VIEW_TYPE_2D,
			m_surfaceFormat,
			{ VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A },
			{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 } };


		check(vkCreateImageView(m_device_context->get_device(), &viewCreateInfo, nullptr, &entry.imageView), "Create Image View error");

		VkImageSubresourceRange range = { 0 };
		range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		range.baseMipLevel = 0;
		range.levelCount = VK_REMAINING_MIP_LEVELS;
		range.baseArrayLayer = 0;
		range.layerCount = VK_REMAINING_ARRAY_LAYERS;

		VkImageMemoryBarrier memBarrier = { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
		memBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		memBarrier.dstAccessMask = 0;
		memBarrier.srcAccessMask = 0;
		memBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		memBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		memBarrier.image = entry.image;
		memBarrier.subresourceRange = range;

		m_barriers[i] = memBarrier;

	}
}

/*
* update SwapChain 
* if SwapChain is empty
* create new SwapChain
*/
VkExtent2D van::SwapChain::update(int width, int height, bool vsync)
{
	m_changeID++;

	check(wait_idle(), "wait Idle error");

	auto surface_capabilities = m_device_context->get_vulkan_context()->get_swapchain_coontext().
		swap_chian_support_details.capabilities;
	std::vector<VkPresentModeKHR> present_modes = m_device_context->get_vulkan_context()->get_swapchain_coontext().
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

	VkSwapchainKHR oldSwapchain = m_swapchain;
	//update

	VkSwapchainCreateInfoKHR swapchain_create_info = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
	swapchain_create_info.surface = m_surface;
	swapchain_create_info.minImageCount = desiredNumberOfSwapchainImages;
	swapchain_create_info.imageFormat = m_surfaceFormat;
	swapchain_create_info.imageColorSpace = m_surfaceColor;
	swapchain_create_info.imageExtent = swapchainExtent;
	swapchain_create_info.imageUsage = m_imageUsage;
	swapchain_create_info.preTransform = preTransform;
	swapchain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchain_create_info.imageArrayLayers = 1;
	swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchain_create_info.queueFamilyIndexCount = 1;
	swapchain_create_info.pQueueFamilyIndices = &m_queueFamilyIndex;
	swapchain_create_info.presentMode = swapchainPresentMode;
	swapchain_create_info.oldSwapchain = oldSwapchain;
	swapchain_create_info.clipped = true;

	check(vkCreateSwapchainKHR(m_device, &swapchain_create_info, nullptr, &m_swapchain),"Create SwapChian error");

  // If we just re-created an existing swapchain, we should destroy the old
  // swapchain at this point.
  // Note: destroying the swapchain also cleans up all its associated
  // presentable images once the platform is done with them.

	if (oldSwapchain != VK_NULL_HANDLE)
	{
		for (auto it : m_entries)
		{
			vkDestroyImageView(m_device, it.imageView, nullptr);
		}

		for (auto it : m_semaphores)
		{
			vkDestroySemaphore(m_device, it.readSemaphore, nullptr);
			vkDestroySemaphore(m_device, it.writtenSemaphore, nullptr);
		}

		vkDestroySwapchainKHR(m_device, oldSwapchain, nullptr);
	}

	update_swpachain_images();

	//semaphores settings
	m_semaphores.resize(getSemaphoreCycleCount());


	for (uint32_t i = 0; i < getSemaphoreCycleCount(); i++)
	{
		SemaphoreEntry& entry = m_semaphores[i];
		// semaphore
		VkSemaphoreCreateInfo semCreateInfo = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };

		vkCreateSemaphore(m_device, &semCreateInfo, nullptr, &entry.readSemaphore);
	
		vkCreateSemaphore(m_device, &semCreateInfo, nullptr, &entry.writtenSemaphore);
		

		
	}

	m_updateWidth = width;
	m_updateHeight = height;
	m_vsync = vsync;
	m_extent = swapchainExtent;

	m_currentSemaphore = 0;
	m_currentImage = 0;

	return swapchainExtent;
}




bool van::SwapChain::acquireAutoResize(int width, int height, bool* pRecreated, SwapChainAcquireState* pOut)
{
	return acquireCustom(VK_NULL_HANDLE, width, height, pRecreated, pOut);
}

bool van::SwapChain::acquire(bool* pRecreated, SwapChainAcquireState* pOut)
{
	return acquireCustom(VK_NULL_HANDLE, m_updateWidth, m_updateHeight, pRecreated, pOut);
}

bool van::SwapChain::init(VkDevice device, VkPhysicalDevice physicalDevice, VkQueue queue, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkFormat format, VkImageUsageFlags imageUsage)
{

	auto  context = this->m_device_context->get_vulkan_context();

	return false;
}

void van::SwapChain::create_swap_chain()
{

	VkSharingMode share_mode;
	uint32_t queue_famliy_index_count;

	if (m_device_context->get_vulkan_context()->is_same_Graphic_PresentQueueFamily())
	{
		share_mode = VK_SHARING_MODE_EXCLUSIVE;
		queue_famliy_index_count = 0;
	}




	VkSwapchainCreateInfoKHR swapChainInfo = {
	};

	swapChainInfo.imageSharingMode = share_mode;
	swapChainInfo.imageFormat = m_format;

}

bool van::SwapChain::acquireCustom(VkSemaphore argSemaphore, int width, int height, bool* pRecreated, SwapChainAcquireState* pOut)
{
	bool didRecreate = false;

	if (width != m_updateWidth || height != m_updateHeight)
	{
		deinitResources();
		update(width, height);
		m_updateWidth = width;
		m_updateHeight = height;
		didRecreate = true;
	}
	if (pRecreated != nullptr)
	{
		*pRecreated = didRecreate;
	}

	for (int i = 0; i < 2; i++)
	{
		VkSemaphore semaphore = argSemaphore ? argSemaphore : getActiveReadSemaphore();
		VkResult    result;
		result = vkAcquireNextImageKHR(m_device, m_swapchain, UINT64_MAX, semaphore, (VkFence)VK_NULL_HANDLE, &m_currentImage);

		if (result == VK_SUCCESS)
		{
			if (pOut != nullptr)
			{
				pOut->image = getActiveImage();
				pOut->view = getActiveImageView();
				pOut->index = getActiveImageIndex();
				pOut->waitSem = getActiveReadSemaphore();
				pOut->signalSem = getActiveWrittenSemaphore();
			}
			return true;
		}
		else if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		{
			deinitResources();
			update(width, height, m_vsync);
		}
		else
		{
			return false;
		}
	}

	return false;
}


namespace van {


	void SwapChain::deinitResources()
	{
		if (!m_device)
			return;

		VkResult result = wait_idle();
	

		for (auto it : m_entries)
		{
			vkDestroyImageView(m_device, it.imageView, nullptr);
		}

		for (auto it : m_semaphores)
		{
			vkDestroySemaphore(m_device, it.readSemaphore, nullptr);
			vkDestroySemaphore(m_device, it.writtenSemaphore, nullptr);
		}

		if (m_swapchain)
		{
			vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
			m_swapchain = VK_NULL_HANDLE;
		}

		m_entries.clear();
		m_barriers.clear();
	}


	VkSemaphore SwapChain::getActiveWrittenSemaphore() const
	{
		return m_semaphores[(m_currentSemaphore % getSemaphoreCycleCount())].writtenSemaphore;
	}

	VkSemaphore SwapChain::getActiveReadSemaphore() const
	{
		return m_semaphores[(m_currentSemaphore % getSemaphoreCycleCount())].readSemaphore;
	}

	VkImage SwapChain::getActiveImage() const
	{
		return m_entries[m_currentImage].image;
	}

	VkImageView SwapChain::getActiveImageView() const
	{
		return m_entries[m_currentImage].imageView;
	}

	VkImage SwapChain::getImage(uint32_t i) const
	{
		if (i >= m_imageCount)
			return nullptr;
		return m_entries[i].image;
	}

	uint32_t SwapChain::getChangeID() const
	{
		return m_changeID;
	}

	VkImageView SwapChain::getImageView(uint32_t i) const
	{
		if (i >= m_imageCount)
			return nullptr;
		return m_entries[i].imageView;
	}

}


void van::SwapChain::present(VkQueue queue)
{
	VkResult         result;
	VkPresentInfoKHR presentInfo;

	presentCustom(presentInfo);

	result = vkQueuePresentKHR(queue, &presentInfo);
}

void van::SwapChain::presentCustom(VkPresentInfoKHR& presentInfo)
{
	VkSemaphore& written = m_semaphores[(m_currentSemaphore % getSemaphoreCycleCount())].writtenSemaphore;

	presentInfo = { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
	presentInfo.swapchainCount = 1;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &written;
	presentInfo.pSwapchains = &m_swapchain;
	presentInfo.pImageIndices = &m_currentImage;

	m_currentSemaphore++;
}






void van::SwapChain::cmdUpdateBarriers(VkCommandBuffer cmd) const
{
	vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, nullptr, 0,
		nullptr, m_imageCount, m_barriers.data());
}
