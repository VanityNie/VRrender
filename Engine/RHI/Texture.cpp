#include "Texture.h"

void Texture::CreateImageView()
{

    VkImageViewCreateInfo viewInfo = {};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = m_image->GetImage();
    viewInfo.viewType = GetVulkanViewType();
    viewInfo.format = GetVulkanFormat(m_specification.Format);
    viewInfo.subresourceRange.aspectMask = GetVulkanAspectMask();
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = m_specification.MipLevels;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = (m_specification.Depth > 1) ? 1 : m_specification.LayerCount;

    if (vkCreateImageView(m_device->get_device(), &viewInfo, nullptr, &m_imageView) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create texture image view!");
    }
	
	
}

void Texture::LoadFromFile(const std::string& filepath, bool generate_mipmaps)
{

    int width, height, channels;
    stbi_uc* pixels = stbi_load(filepath.c_str(), &width, &height, &channels, STBI_rgb_alpha);

    if (!pixels) {
        LOG_ERROR("Failed to load texture image!")
    }

    if (generate_mipmaps)
    {
        m_specification.MipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;

    }

    
    //create image and allocate memory
    Create(m_device->m_memory_alloactor, m_device->get_device());




    VkBufferUsageFlags m_usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    VmaMemoryUsage m_memoryUsage = VMA_MEMORY_USAGE_CPU_TO_GPU;
    VkDeviceSize size = width * height * 4;

    Buffer stage_buffer(m_device.get(), size, m_usage, m_memoryUsage);

    void* data;

    //stage buffer get image data
    stage_buffer.CopyToBuffer(pixels, size);


    stbi_image_free(pixels);

    m_specification.Width = width;
    m_specification.Height = height;
  

    //todo copy buffer data to texture command submit 

    VkImageSubresourceRange subresourceRange = {};

    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = m_specification.MipLevels;
    subresourceRange.layerCount = 1;

    
    //todo cpoy command buffer init

    VkCommandBuffer copy_cmd;



    auto image_usage = GetVulkanUsageFlags();

    tools::set_image_layout(
        copy_cmd,
        m_image.get()->GetImage(),
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        subresourceRange
        );

    //
    VkBufferImageCopy region{};

    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = GetVulkanAspectMask();
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageExtent.width = width;
    region.imageExtent.height = height;
    region.imageExtent.depth = 1;




    vkCmdCopyBufferToImage(copy_cmd, stage_buffer.GetBuffer(), m_image.get()->GetImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

   


    

}

void Texture::Create(VmaAllocator allocator, VkDevice device)
{
 

    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = (m_specification.Depth > 1) ? VK_IMAGE_TYPE_3D : VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = m_specification.Width;
    imageInfo.extent.height = m_specification.Height;
    imageInfo.extent.depth = (m_specification.Depth > 1) ? m_specification.Depth : 1;
    imageInfo.mipLevels = m_specification.MipLevels;
    imageInfo.arrayLayers = (m_specification.Depth > 1) ? 1 : m_specification.LayerCount;
    

    imageInfo.format = GetVulkanFormat(m_specification.Format);
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    //usage 
    imageInfo.usage = GetVulkanUsageFlags();
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

   

    VmaAllocationCreateInfo allocInfo = {};
    allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;


    m_image = std::make_unique<Image>( allocator, imageInfo, VMA_MEMORY_USAGE_GPU_ONLY );
    
    if (m_specification.IsCubemap)
        imageInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
}

void Texture::Create(VmaAllocator allocator,const VkImageCreateInfo& imageInfo, const VmaMemoryUsage& usgae)
{
    m_image = std::make_unique<Image>(allocator, imageInfo, VMA_MEMORY_USAGE_GPU_ONLY);
}





VkImageUsageFlags Texture::GetVulkanUsageFlags()
{
    VkImageUsageFlags usage = 0;
    if (m_specification.IsUsageSampled) usage |= VK_IMAGE_USAGE_SAMPLED_BIT;
    if (m_specification.IsUsageStorage) usage |= VK_IMAGE_USAGE_STORAGE_BIT;
    if (m_specification.IsUsageTransfer) usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    if (m_specification.IsUsageRenderTarget) usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    return usage;
}

VkImageViewType Texture::GetVulkanViewType()
{
    if (m_specification.IsCubemap)
        return m_specification.LayerCount > 1 ? VK_IMAGE_VIEW_TYPE_CUBE_ARRAY : VK_IMAGE_VIEW_TYPE_CUBE;
    else if (m_specification.Depth > 1)
        return VK_IMAGE_VIEW_TYPE_3D;
    else
        return m_specification.LayerCount > 1 ? VK_IMAGE_VIEW_TYPE_2D_ARRAY : VK_IMAGE_VIEW_TYPE_2D;
}

VkImageAspectFlags Texture::GetVulkanAspectMask()
{
    switch (m_specification.Format)
    {
    case TextureFormat::Depth24Stencil8:
        return VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    case TextureFormat::Depth32F:
        return VK_IMAGE_ASPECT_DEPTH_BIT;
    default:
        return VK_IMAGE_ASPECT_COLOR_BIT;
    }
}