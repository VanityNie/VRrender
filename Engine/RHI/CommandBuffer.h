#include"pch.h"

class CommandBuffer
{


private:
	VkCommandBuffer handle = VK_NULL_HANDLE;
	enum class CommandBufferState { RECORDING, STOPPED };

	std::shared_ptr<Device> m_device;
	CommandBufferState state = CommandBufferState::STOPPED;
	QueueType type;
	uint32_t curr_tid;

public:

	CommandBuffer(Device* device, bool begin = false, VkCommandBufferUsageFlags begin_flags = 0,
		QueueType type = QueueType::GFX, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);

	void begin(VkCommandBufferUsageFlags begin_flags = 0);
	void submit(bool wait_fences = true, bool queue_wait_idle = true);


};