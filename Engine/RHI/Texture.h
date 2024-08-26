
#include"pch.h"






static VkFormat GetVulkanFormat(TextureFormat format)
{
    switch (format)
    {
    case TextureFormat::RGBA8: return VK_FORMAT_R8G8B8A8_UNORM;
    case TextureFormat::RGBA16F: return VK_FORMAT_R16G16B16A16_SFLOAT;
    case TextureFormat::RGBA32F: return VK_FORMAT_R32G32B32A32_SFLOAT;
    case TextureFormat::R8: return VK_FORMAT_R8_UNORM;
    case TextureFormat::R16F: return VK_FORMAT_R16_SFLOAT;
    case TextureFormat::R32F: return VK_FORMAT_R32_SFLOAT;
    case TextureFormat::Depth24Stencil8: return VK_FORMAT_D24_UNORM_S8_UINT;
    case TextureFormat::Depth32F: return VK_FORMAT_D32_SFLOAT;
    default: return VK_FORMAT_R8G8B8A8_UNORM;
    }
}



class Texture
{
   
protected:
    std::unique_ptr<Image> m_image;
    VkImageView m_imageView = VK_NULL_HANDLE;
    VkSampler m_sampler = VK_NULL_HANDLE;
    TextureSpecification m_specification;
    //VkDevice* m_device = VK_NULL_HANDLE;
    VmaAllocator m_allocator = VK_NULL_HANDLE;
     VmaAllocation m_allocation = VK_NULL_HANDLE;
     std::shared_ptr<Device> m_device;
    void CreateImageView();
    void CreateSampler();

public:

    void LoadFromFile(const std::string& filepath);
    Texture(const TextureSpecification& texture_spec, Device  device) : m_specification(texture_spec), m_device(std::make_shared<Device>(device) )
    {
     
       
    }


    void Create(VmaAllocator allocator, VkDevice device);
    void Create(VmaAllocator allocator,const  VkImageCreateInfo& imageInfo, const VmaMemoryUsage& usgae);

    void setSampler(VkSampler sampler) {
        m_sampler = sampler;
    }



    VkImageUsageFlags GetVulkanUsageFlags();
    VkImageViewType GetVulkanViewType();
    VkImageAspectFlags GetVulkanAspectMask();
   
};



class TextureCube : public Texture
{
public:
    TextureCube(const TextureSpecification& spec) : Texture(spec)
    {
        m_specification.IsCubemap = true;
        m_specification.LayerCount = 6;
    }
};

class Texture3D : public Texture
{
public:
    Texture3D(const TextureSpecification& spec) : Texture(spec) {}
};

class TextureArray : public Texture
{
public:
    TextureArray(const TextureSpecification& spec) : Texture(spec) {}
};