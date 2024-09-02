#include "CommandBuffer.h"

CommandBuffer::CommandBuffer(Device* device, bool begin, VkCommandBufferUsageFlags begin_flags, QueueType type, VkCommandBufferLevel level)
{


	this->m_device  =  std::make_shared<Device> (device);
	this->type = type;

	

	//todo cv mutex to promise command buffer safe
	curr_tid = --VulkanSyncronization::available_command_pools;
	auto cmd_buf_allocate_info = tools::command_buffer_allocate_info(device->get_cmd_pools()[curr_tid], level, 1);
	check(vkAllocateCommandBuffers(device->get_device(), &cmd_buf_allocate_info, &handle), "Could not allocate command buffer");


	if(begin)
	{
		auto begin_info = tools ::command_buffer_begin_info(begin_flags);
		check(vkBeginCommandBuffer(handle, &begin_info), "Could not begin command buffer");

		state = CommandBufferState::RECORDING;
	}


}

void CommandBuffer::begin(VkCommandBufferUsageFlags begin_flags)
{
	assert(state != CommandBufferState::RECORDING);
	
	std::unique_lock<std::mutex> cv_lock;
	VulkanSyncronization::cv.wait(cv_lock, [&] {return VulkanSyncronization::available_command_pools > 0; });

	--VulkanSyncronization::available_command_pools;

	auto begin_info = tools::command_buffer_begin_info(begin_flags);
	check(vkBeginCommandBuffer(handle, &begin_info), "Could not begin command buffer");

	state = CommandBufferState::RECORDING;
}

void CommandBuffer::submit(bool wait_fences, bool queue_wait_idle)
{

	check(vkEndCommandBuffer(handle), "Failed to end command buffer");
	++VulkanSyncronization::available_command_pools;
	VulkanSyncronization::cv.notify_one();
	state = CommandBufferState::STOPPED;
	VkSubmitInfo submit_info = tools::submit_info();
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &handle;

	VulkanSyncronization::queue_mutex.lock();

	if (wait_fences)
	{

		VkFenceCreateInfo fence_info = tools::fence_create_info(0);
		VkFence fence;

		check(vkCreateFence(m_device->get_device(), &fence_info, nullptr, &fence), "Fence failed to create");
		check(vkQueueSubmit(m_device->get_spec_queue(type), 1, &submit_info, fence), "Queue submission error");
		check(vkWaitForFences(m_device->get_device(), 1, &fence, VK_TRUE, 100000000000), "Fence wait error");
		vkDestroyFence(m_device->get_device(), fence, nullptr);
	}
	else {
		check(vkQueueSubmit(m_device->get_spec_queue(type), 1, &submit_info, VK_NULL_HANDLE), "Queue submission error");
	}

	if (queue_wait_idle) {
		check(vkQueueWaitIdle(m_device->get_spec_queue(type)), "Queue wait error! Check previous submissions");
	}

	VulkanSyncronization::queue_mutex.unlock();
}

CommandBuffer::~CommandBuffer()
{

	if (handle == nullptr)
	{
		return;
	}

	if (state == CommandBufferState::RECORDING)
	{
		check(vkEndCommandBuffer(handle), "Failed to end command buffer");
		++VulkanSyncronization::available_command_pools;
		VulkanSyncronization::cv.notify_one();
	}

	vkFreeCommandBuffers(m_device->get_device(), m_device->get_cmd_pools()[curr_tid], 1, &handle);
}


