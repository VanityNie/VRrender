#include "image.h"


Image::Image(VmaAllocator allocator, const VkImageCreateInfo& imageInfo, VmaMemoryUsage memoryUsage):m_allocator(allocator)
{
    VmaAllocationCreateInfo allocInfo = {};
    allocInfo.usage = memoryUsage;

    VkResult result = vmaCreateImage(m_allocator, &imageInfo, &allocInfo, &m_image, &m_allocation, nullptr);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create image!");
    }

    m_createInfo = imageInfo;
}

Image::~Image()
{

    if (m_image != VK_NULL_HANDLE)
    {
        vmaDestroyImage(m_allocator, m_image, m_allocation);
    }
}
