#include "CommandBuffer.h"

CommandBuffer::CommandBuffer(Device* device, bool begin, VkCommandBufferUsageFlags begin_flags, QueueType type, VkCommandBufferLevel level)
{


	this->m_device = device;
	this->type = type;


}
