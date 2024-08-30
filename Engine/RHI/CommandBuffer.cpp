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
