#include "graphics/texture/texture_1d_array.hpp"

namespace vg
{
    Texture1DArray::Texture1DArray(vk::Format format
        , Bool32 mipMap
        , uint32_t width
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
        m_type = TextureType::TEX_1D_ARRAY;
        m_width = width;
        m_arrayLength = arraylength;
        m_allAspectFlags = vk::ImageAspectFlagBits::eColor;
        m_usageFlags = vk::ImageUsageFlagBits::eSampled;
        m_layout = vk::ImageLayout::eShaderReadOnlyOptimal;
        _init(VG_TRUE);
    }

    Texture1DArray::~Texture1DArray()
    {
    }

    uint32_t Texture1DArray::getWidth() const
    {
        return m_width;
    }

    uint32_t Texture1DArray::getArrayLength() const
    {
        return m_arrayLength;
    }

    void Texture1DArray::applyData(const TextureDataInfo &layoutInfo
            , const void *memory
            , uint32_t size
            , Bool32 cacheMemory
            , Bool32 createMipmaps)
    {
        _applyData(layoutInfo, memory, size, cacheMemory, createMipmaps);
    }
}