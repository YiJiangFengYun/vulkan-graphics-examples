#include "graphics/texture/texture_2d.hpp"

namespace vg
{
    Texture2D::Texture2D(vk::Format format
        , Bool32 mipMap
        , uint32_t width
        , uint32_t height
        , vk::ImageUsageFlags additionalUsage
        , Bool32 cacheMemory
        , Bool32 createMipmaps
        )
        : Texture(format
        , mipMap
        , additionalUsage
        , cacheMemory
        , createMipmaps
        )
    {
        m_type = TextureType::TEX_2D;
        m_width = width;
        m_height = height;
        m_allAspectFlags = vk::ImageAspectFlagBits::eColor;
        m_usageFlags = vk::ImageUsageFlagBits::eSampled;
        m_layout = vk::ImageLayout::eShaderReadOnlyOptimal;
        _init(VG_TRUE);
    }

    Texture2D::~Texture2D()
    {
    }

    uint32_t Texture2D::getWidth() const
    {
        return m_width;
    }

    uint32_t Texture2D::getHeight() const
    {
        return m_height;
    }

    void Texture2D::applyData(const TextureDataInfo &layoutInfo
            , const void *memory
            , uint32_t size
            , Bool32 cacheMemory
            , Bool32 createMipmaps)
    {
        _applyData(layoutInfo, memory, size, cacheMemory, createMipmaps);
    }


    Texture2DColorAttachment::Texture2DColorAttachment(vk::Format format
        , uint32_t width
        , uint32_t height
        , Bool32 isInputUsage
        , vk::ImageUsageFlags additionalUsage
        , Bool32 defaultImageView
        , Bool32 defaultSampler
        )
        : Texture(format
        , false
        , additionalUsage
        , defaultImageView
        , defaultSampler
        )
        , BaseColorAttachment(isInputUsage)
    {
        m_type = TextureType::TEX_2D_COLOR_ATTACHMENT;
        m_width = width;
        m_height = height;
        m_allAspectFlags = vk::ImageAspectFlagBits::eColor;
        m_usageFlags = vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eColorAttachment;
        m_layout = vk::ImageLayout::eShaderReadOnlyOptimal;
        if (isInputUsage)
        {
            m_usageFlags |= vk::ImageUsageFlagBits::eInputAttachment;
        }
        _init(VG_FALSE);
    }

    Texture2DColorAttachment::~Texture2DColorAttachment()
    {
    }

    uint32_t Texture2DColorAttachment::getWidth() const
    {
        return m_width;
    }

    uint32_t Texture2DColorAttachment::getHeight() const
    {
        return m_height;
    }

    void Texture2DColorAttachment::applyData(const TextureDataInfo &layoutInfo
            , const void *memory
            , uint32_t size
            , Bool32 cacheMemory
            , Bool32 createMipmaps)
    {
        _applyData(layoutInfo, memory, size, cacheMemory, createMipmaps);
    }

    uint32_t Texture2DColorAttachment::getColorAttachmentWidth() const
    {
        return m_width;
    }
        
    uint32_t Texture2DColorAttachment::getColorAttachmentHeight() const
    {
        return m_height;
    }
    
    uint32_t Texture2DColorAttachment::getColorAttachmentLayers() const
    {
        return 1u;
    }

    const vk::Format Texture2DColorAttachment::getColorAttachmentFormat() const
    {
        return m_format;
    }

    const vk::ImageLayout Texture2DColorAttachment::getColorAttachmentLayout() const
    {
        return m_layout;
    }

    const vk::ImageView *Texture2DColorAttachment::getColorAttachmentImageView() const
    {
        if (m_pImageView == nullptr)throw std::runtime_error("Image view of texture don't exist!");
        return m_pImageView->getImageView();
    }

    void Texture2DColorAttachment::_init(Bool32 importContent)
    {
        Texture::_init(importContent);
    }

    Texture2DDepthStencilAttachment::Texture2DDepthStencilAttachment(vk::Format format
        , uint32_t width
        , uint32_t height
        , Bool32 isInputUsage
        , vk::ImageUsageFlags additionalUsage
        , Bool32 defaultImageView
        , Bool32 defaultSampler
        )
        : Texture(format
        , false
        , additionalUsage
        , defaultImageView
        , defaultSampler
        )
        , BaseDepthStencilAttachment(isInputUsage)
    {
        m_type = TextureType::TEX_2D_DEPTH_STENCIL_ATTACHMENT;
        m_width = width;
        m_height = height;
        m_allAspectFlags = vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
        m_usageFlags = vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eDepthStencilAttachment;
        m_layout = vk::ImageLayout::eDepthStencilReadOnlyOptimal;
        if (isInputUsage)
        {
            m_usageFlags |= vk::ImageUsageFlagBits::eInputAttachment;
        }
        _checkDepthFormat();
        _init(VG_FALSE);
    }

    Texture2DDepthStencilAttachment::~Texture2DDepthStencilAttachment()
    {
    }

    uint32_t Texture2DDepthStencilAttachment::getWidth() const
    {
        return m_width;
    }

    uint32_t Texture2DDepthStencilAttachment::getHeight() const
    {
        return m_height;
    }

    uint32_t Texture2DDepthStencilAttachment::getDepthStencilAttachmentWidth() const
    {
        return m_width;
    }

    uint32_t Texture2DDepthStencilAttachment::getDepthStencilAttachmentHeight() const
    {
        return m_height;
    }
    
    uint32_t Texture2DDepthStencilAttachment::getDepthStencilAttachmentLayers() const
    {
        return 1u;
    }

    const vk::Format Texture2DDepthStencilAttachment::getDepthStencilAttachmentFormat() const
    {
        return m_format;
    }

    const vk::ImageLayout Texture2DDepthStencilAttachment::getDepthStencilAttachmentLayout() const
    {
        return m_layout;
    }

    const vk::ImageView *Texture2DDepthStencilAttachment::getDepthStencilAttachmentImageView() const
    {
        return m_pImageView->getImageView();
    }

    void Texture2DDepthStencilAttachment::applyData(const TextureDataInfo &layoutInfo
            , const void *memory
            , uint32_t size
            , Bool32 cacheMemory
            , Bool32 createMipmaps)
    {
        _applyData(layoutInfo, memory, size, cacheMemory, createMipmaps);
    }

    void Texture2DDepthStencilAttachment::_init(Bool32 importContent)
    {
        Texture::_init(importContent);
    }

    void Texture2DDepthStencilAttachment::_checkDepthFormat()
    {
        const auto &pPhysicalDevice = pApp->getPhysicalDevice();
        const auto &props = pPhysicalDevice->getFormatProperties(m_format);
        if ((props.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment) !=
            vk::FormatFeatureFlagBits::eDepthStencilAttachment)
        {
            throw std::runtime_error("The format don't support as depth stencil attachment!");
        }
        if ((props.optimalTilingFeatures & vk::FormatFeatureFlagBits::eSampledImage) !=
            vk::FormatFeatureFlagBits::eSampledImage) 
        {
            throw std::runtime_error("The format don't support to sample from this image!");
        }
    }

    Texture2DDepthAttachment::Texture2DDepthAttachment(vk::Format format
        , uint32_t width
        , uint32_t height
        , Bool32 isInputUsage
        , vk::ImageUsageFlags additionalUsage
        , Bool32 defaultImageView
        , Bool32 defaultSampler
        )
        : Texture(format
        , false
        , additionalUsage
        , defaultImageView
        , defaultSampler
        )
        , BaseDepthStencilAttachment(isInputUsage)
    {
        m_type = TextureType::TEX_2D_DEPTH_ATTACHMENT;
        m_width = width;
        m_height = height;
        m_allAspectFlags = vk::ImageAspectFlagBits::eDepth;
        m_usageFlags = vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eDepthStencilAttachment;
        m_layout = vk::ImageLayout::eDepthStencilReadOnlyOptimal;
        if (isInputUsage)
        {
            m_usageFlags |= vk::ImageUsageFlagBits::eInputAttachment;
        }
        _checkDepthFormat();
        _init(VG_FALSE);
    }

    Texture2DDepthAttachment::~Texture2DDepthAttachment()
    {
    }

    uint32_t Texture2DDepthAttachment::getWidth() const
    {
        return m_width;
    }

    uint32_t Texture2DDepthAttachment::getHeight() const
    {
        return m_height;
    }

    uint32_t Texture2DDepthAttachment::getDepthStencilAttachmentWidth() const
    {
        return m_width;
    }

    uint32_t Texture2DDepthAttachment::getDepthStencilAttachmentHeight() const
    {
        return m_height;
    }
    
    uint32_t Texture2DDepthAttachment::getDepthStencilAttachmentLayers() const
    {
        return 1u;
    }

    const vk::Format Texture2DDepthAttachment::getDepthStencilAttachmentFormat() const
    {
        return m_format;
    }

    const vk::ImageLayout Texture2DDepthAttachment::getDepthStencilAttachmentLayout() const
    {
        return m_layout;
    }

    const vk::ImageView *Texture2DDepthAttachment::getDepthStencilAttachmentImageView() const
    {
        return m_pImageView->getImageView();
    }

    void Texture2DDepthAttachment::applyData(const TextureDataInfo &layoutInfo
            , const void *memory
            , uint32_t size
            , Bool32 cacheMemory
            , Bool32 createMipmaps)
    {
        _applyData(layoutInfo, memory, size, cacheMemory, createMipmaps);
    }

    void Texture2DDepthAttachment::_init(Bool32 importContent)
    {
        Texture::_init(importContent);
    }

    void Texture2DDepthAttachment::_checkDepthFormat()
    {
        const auto &pPhysicalDevice = pApp->getPhysicalDevice();
        const auto &props = pPhysicalDevice->getFormatProperties(m_format);
        if ((props.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment) !=
            vk::FormatFeatureFlagBits::eDepthStencilAttachment)
        {
            throw std::runtime_error("The format don't support as depth stencil attachment!");
        }
        if ((props.optimalTilingFeatures & vk::FormatFeatureFlagBits::eSampledImage) !=
            vk::FormatFeatureFlagBits::eSampledImage) 
        {
            throw std::runtime_error("The format don't support to sample from this image!");
        }
    }
}