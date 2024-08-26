#include"pch.h"



enum class TextureFormat
{
    RGBA8,
    RGBA16F,
    RGBA32F,
    R8,
    R16F,
    R32F,
    Depth24Stencil8,
    Depth32F
};

class Image
{

private:
    VmaAllocator m_allocator;
    VkImage m_image = VK_NULL_HANDLE;
    VmaAllocation m_allocation = VK_NULL_HANDLE;
    VkImageCreateInfo m_createInfo;

public:
    VkImage GetImage() const { return m_image; }
    const VkImageCreateInfo& GetCreateInfo() const { return m_createInfo; }


    Image(VmaAllocator allocator, const VkImageCreateInfo& imageInfo, VmaMemoryUsage memoryUsage);
    ~Image();

    Image(const Image&) = delete;
    Image(Image&& other)  noexcept : m_allocator(other.m_allocator), m_image(other.m_image), m_allocation(other.m_allocation), m_createInfo(other.m_createInfo)
    {
        other.m_image = VK_NULL_HANDLE;
        other.m_allocation = VK_NULL_HANDLE;
    }

    Image& operator=(Image&& other) noexcept
    {
        if (this != &other)
        {
            vmaDestroyImage(m_allocator, m_image, m_allocation);
            m_allocator = other.m_allocator;
            m_image = other.m_image;
            m_allocation = other.m_allocation;
            m_createInfo = other.m_createInfo;
            other.m_image = VK_NULL_HANDLE;
            other.m_allocation = VK_NULL_HANDLE;
        }
        return *this;
    }
};