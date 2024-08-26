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

enum class TexturePreset
{
    Color2D,
    Color2DArray,
    Cubemap,
    Depth,
    Volume3D,
    HDR2D,
    NormalMap,
    ShadowMap
};

struct TextureSpecification
{
    bool IsUsageSampled = true;
    bool IsUsageStorage = false;
    bool IsUsageTransfer = true;
    bool IsUsageRenderTarget = false;
    bool PerformTransition = true;
    bool IsCubemap = false;
    uint32_t Width = 0;
    uint32_t Height = 0;
    uint32_t Depth = 1;
    uint32_t MipLevels = 1;
    uint32_t LayerCount = 1;
    TextureFormat Format = TextureFormat::RGBA8;

    static TextureSpecification CreatePreset(TexturePreset preset, uint32_t width = 0, uint32_t height = 0)
    {
        TextureSpecification spec;
        spec.Width = width;
        spec.Height = height;

        switch (preset)
        {
        case TexturePreset::Color2D:
            spec.Format = TextureFormat::RGBA8;
            spec.IsUsageRenderTarget = true;
            break;
        case TexturePreset::Color2DArray:
            spec.Format = TextureFormat::RGBA8;
            spec.LayerCount = 10; // 默认10层，可以根据需要调整
            spec.IsUsageRenderTarget = true;
            break;
        case TexturePreset::Cubemap:
            spec.Format = TextureFormat::RGBA8;
            spec.IsCubemap = true;
            spec.LayerCount = 6;
            break;
        case TexturePreset::Depth:
            spec.Format = TextureFormat::Depth24Stencil8;
            spec.IsUsageRenderTarget = true;
            break;
        case TexturePreset::Volume3D:
            spec.Format = TextureFormat::R8;
            spec.Depth = 256; // 默认深度，可以根据需要调整
            break;
        case TexturePreset::HDR2D:
            spec.Format = TextureFormat::RGBA16F;
            spec.IsUsageRenderTarget = true;
            break;
        case TexturePreset::NormalMap:
            spec.Format = TextureFormat::RGBA8;
            spec.IsUsageRenderTarget = false;
            break;
        case TexturePreset::ShadowMap:
            spec.Format = TextureFormat::Depth32F;
            spec.IsUsageRenderTarget = true;
            break;
        }

        return spec;
    }
};


enum class TextureFilterMode
{
    Nearest,
    Linear
};

enum class TextureWrapMode
{
    Repeat,
    ClampToEdge,
    ClampToBorder,
    MirroredRepeat
};

struct SamplerSpecification
{
    TextureFilterMode MinFilter = TextureFilterMode::Linear;
    TextureFilterMode MagFilter = TextureFilterMode::Linear;
    TextureFilterMode MipmapMode = TextureFilterMode::Linear;
    TextureWrapMode WrapU = TextureWrapMode::Repeat;
    TextureWrapMode WrapV = TextureWrapMode::Repeat;
    TextureWrapMode WrapW = TextureWrapMode::Repeat;
    float MaxAnisotropy = 1.0f;
    bool EnableCompare = false;
    VkCompareOp CompareOp = VK_COMPARE_OP_ALWAYS;
    float MinLod = 0.0f;
    float MaxLod = VK_LOD_CLAMP_NONE;
    float MipLodBias = 0.0f;
    VkBorderColor BorderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
};



// 辅助转换函数
VkFilter ConvertFilterMode(TextureFilterMode mode)
{
    switch (mode)
    {
    case TextureFilterMode::Nearest: return VK_FILTER_NEAREST;
    case TextureFilterMode::Linear: return VK_FILTER_LINEAR;
    default: return VK_FILTER_LINEAR;
    }
}

VkSamplerMipmapMode ConvertMipmapMode(TextureFilterMode mode)
{
    switch (mode)
    {
    case TextureFilterMode::Nearest: return VK_SAMPLER_MIPMAP_MODE_NEAREST;
    case TextureFilterMode::Linear: return VK_SAMPLER_MIPMAP_MODE_LINEAR;
    default: return VK_SAMPLER_MIPMAP_MODE_LINEAR;
    }
}

VkSamplerAddressMode ConvertWrapMode(TextureWrapMode mode)
{
    switch (mode)
    {
    case TextureWrapMode::Repeat: return VK_SAMPLER_ADDRESS_MODE_REPEAT;
    case TextureWrapMode::ClampToEdge: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    case TextureWrapMode::ClampToBorder: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    case TextureWrapMode::MirroredRepeat: return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
    default: return VK_SAMPLER_ADDRESS_MODE_REPEAT;
    }
}