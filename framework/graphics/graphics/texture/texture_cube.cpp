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

    TextureCubeColorAttachment::TextureCubeColorAttachment(vk::Format format
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
        m_type = TextureType::CUBE_COLOR_ATTACHMENT;
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

    TextureCubeColorAttachment::~TextureCubeColorAttachment()
    {
    }

    uint32_t TextureCubeColorAttachment::getWidth() const
    {
        return m_width;
    }

    uint32_t TextureCubeColorAttachment::getHeight() const
    {
        return m_height;
    }

    void TextureCubeColorAttachment::applyData(const TextureDataInfo &layoutInfo
            , const void *memory
            , uint32_t size
            , Bool32 cacheMemory
            , Bool32 createMipmaps)
    {
        _applyData(layoutInfo, memory, size, cacheMemory, createMipmaps);
    }

    uint32_t TextureCubeColorAttachment::getColorAttachmentWidth() const
    {
        return m_width;
    }
        
    uint32_t TextureCubeColorAttachment::getColorAttachmentHeight() const
    {
        return m_height;
    }
    
    uint32_t TextureCubeColorAttachment::getColorAttachmentLayers() const
    {
        return m_arrayLayers;
    }

    const vk::Format TextureCubeColorAttachment::getColorAttachmentFormat() const
    {
        return m_format;
    }

    const vk::ImageLayout TextureCubeColorAttachment::getColorAttachmentLayout() const
    {
        return m_layout;
    }

    const vk::ImageView *TextureCubeColorAttachment::getColorAttachmentImageView() const
    {
        if (m_pImageView == nullptr)throw std::runtime_error("Image view of texture don't exist!");
        return m_pImageView->getImageView();
    }

    void TextureCubeColorAttachment::_init(Bool32 importContent)
    {
        Texture::_init(importContent);
    }


    TextureCubeDepthStencilAttachment::TextureCubeDepthStencilAttachment(vk::Format format
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
        m_type = TextureType::CUBE_DEPTH_STENCIL_ATTACHMENT;
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

    TextureCubeDepthStencilAttachment::~TextureCubeDepthStencilAttachment()
    {
    }

    uint32_t TextureCubeDepthStencilAttachment::getWidth() const
    {
        return m_width;
    }

    uint32_t TextureCubeDepthStencilAttachment::getHeight() const
    {
        return m_height;
    }

    uint32_t TextureCubeDepthStencilAttachment::getDepthStencilAttachmentWidth() const
    {
        return m_width;
    }

    uint32_t TextureCubeDepthStencilAttachment::getDepthStencilAttachmentHeight() const
    {
        return m_height;
    }
    
    uint32_t TextureCubeDepthStencilAttachment::getDepthStencilAttachmentLayers() const
    {
        return 1u;
    }

    const vk::Format TextureCubeDepthStencilAttachment::getDepthStencilAttachmentFormat() const
    {
        return m_format;
    }

    const vk::ImageLayout TextureCubeDepthStencilAttachment::getDepthStencilAttachmentLayout() const
    {
        return m_layout;
    }

    const vk::ImageView *TextureCubeDepthStencilAttachment::getDepthStencilAttachmentImageView() const
    {
        return m_pImageView->getImageView();
    }

    void TextureCubeDepthStencilAttachment::applyData(const TextureDataInfo &layoutInfo
            , const void *memory
            , uint32_t size
            , Bool32 cacheMemory
            , Bool32 createMipmaps)
    {
        _applyData(layoutInfo, memory, size, cacheMemory, createMipmaps);
    }

    void TextureCubeDepthStencilAttachment::_init(Bool32 importContent)
    {
        Texture::_init(importContent);
    }

    void TextureCubeDepthStencilAttachment::_checkDepthFormat()
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

    TextureCubeDepthAttachment::TextureCubeDepthAttachment(vk::Format format
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
        m_type = TextureType::CUBE_DEPTH_ATTACHMENT;
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

    TextureCubeDepthAttachment::~TextureCubeDepthAttachment()
    {
    }

    uint32_t TextureCubeDepthAttachment::getWidth() const
    {
        return m_width;
    }

    uint32_t TextureCubeDepthAttachment::getHeight() const
    {
        return m_height;
    }

    uint32_t TextureCubeDepthAttachment::getDepthStencilAttachmentWidth() const
    {
        return m_width;
    }

    uint32_t TextureCubeDepthAttachment::getDepthStencilAttachmentHeight() const
    {
        return m_height;
    }
    
    uint32_t TextureCubeDepthAttachment::getDepthStencilAttachmentLayers() const
    {
        return 1u;
    }

    const vk::Format TextureCubeDepthAttachment::getDepthStencilAttachmentFormat() const
    {
        return m_format;
    }

    const vk::ImageLayout TextureCubeDepthAttachment::getDepthStencilAttachmentLayout() const
    {
        return m_layout;
    }

    const vk::ImageView *TextureCubeDepthAttachment::getDepthStencilAttachmentImageView() const
    {
        return m_pImageView->getImageView();
    }

    void TextureCubeDepthAttachment::applyData(const TextureDataInfo &layoutInfo
            , const void *memory
            , uint32_t size
            , Bool32 cacheMemory
            , Bool32 createMipmaps)
    {
        _applyData(layoutInfo, memory, size, cacheMemory, createMipmaps);
    }

    void TextureCubeDepthAttachment::_init(Bool32 importContent)
    {
        Texture::_init(importContent);
    }

    void TextureCubeDepthAttachment::_checkDepthFormat()
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