#include "graphics/texture/texture_2d_array.hpp"

namespace vg
{
    Texture2DArray::Texture2DArray(vk::Format format
        , Bool32 mipMap
        , uint32_t width
        , uint32_t height
        , uint32_t arraylength
        , vk::ImageUsageFlags additionalUsage
        , Bool32 defaultImageView
        , Bool32 defaultSampler
        )
        :Texture(format
        , mipMap
        , additionalUsage
        , defaultImageView
        , defaultSampler
        )
    {
        m_type = TextureType::TEX_2D_ARRAY;
        m_width = width;
        m_height = height;
        m_arrayLength = arraylength;
        m_allAspectFlags = vk::ImageAspectFlagBits::eColor;
        m_usageFlags = vk::ImageUsageFlagBits::eSampled;
        m_layout = vk::ImageLayout::eShaderReadOnlyOptimal;
        _init(VG_TRUE);
    }

    Texture2DArray::~Texture2DArray()
    {
    }

    uint32_t Texture2DArray::getWidth() const
    {
        return m_width;
    }

    uint32_t Texture2DArray::getHeight() const
    {
        return m_height;
    }

    uint32_t Texture2DArray::getArrayLength() const
    {
        return m_arrayLength;
    }

    void Texture2DArray::applyData(const TextureDataInfo &layoutInfo
            , const void *memory
            , uint32_t size
            , Bool32 cacheMemory
            , Bool32 createMipmaps)
    {
        _applyData(layoutInfo, memory, size, cacheMemory, createMipmaps);
    }
}