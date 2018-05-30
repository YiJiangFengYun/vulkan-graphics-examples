#include "graphics/texture/texture_cube.hpp"

namespace vg
{
    TextureCube::TextureCube(vk::Format format
        , Bool32 mipMap
        , uint32_t width
        , uint32_t height
        , vk::ImageUsageFlags additionalUsage
        , Bool32 defaultImageView
        , Bool32 defaultSampler
        )
        : Texture(format
        , mipMap
        , additionalUsage
        , defaultImageView
        , defaultSampler
        )
    {
        m_type = TextureType::CUBE;
        m_width = width;
        m_height = height;
        m_allAspectFlags = vk::ImageAspectFlagBits::eColor;
        m_usageFlags = vk::ImageUsageFlagBits::eSampled;
        m_layout = vk::ImageLayout::eShaderReadOnlyOptimal;
        _init(VG_TRUE);
    }

    TextureCube::~TextureCube()
    {
    }

    uint32_t TextureCube::getWidth() const
    {
        return m_width;
    }

    uint32_t TextureCube::getHeight() const
    {
        return m_height;
    }

    void TextureCube::applyData(const TextureDataInfo &layoutInfo
            , const void *memory
            , uint32_t size
            , Bool32 cacheMemory
            , Bool32 createMipmaps)
    {
        _applyData(layoutInfo, memory, size, cacheMemory, createMipmaps);
    }
}