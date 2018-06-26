#ifndef VG_TEXTURE_CUBE_H
#define VG_TEXTURE_CUBE_H

#include "graphics/texture/texture.hpp"
#include "graphics/texture/attachment.hpp"

namespace vg
{
    class TextureCube : public Texture
    {
    public:
        TextureCube(vk::Format format
            , Bool32 mipmap
            , uint32_t width
            , uint32_t height
            , vk::ImageUsageFlags additionalUsage = vk::ImageUsageFlags()
            , Bool32 defaultImageView = VG_TRUE
            , Bool32 defaultSampler = VG_TRUE
            );
        ~TextureCube();
        uint32_t getWidth() const;
        uint32_t getHeight() const;
        void applyData(const TextureDataInfo &layoutInfo
            , const void *memory
            , uint32_t size
            , Bool32 cacheMemory = VG_FALSE
            , Bool32 createMipmaps = VG_FALSE);
    private:
        TextureCube() = delete;
    };

    class TextureCubeColorAttachment : public Texture, public BaseColorAttachment
    {
    public:
        TextureCubeColorAttachment(vk::Format format
            , uint32_t width
            , uint32_t height
            , Bool32 isInputUsage = VG_FALSE
            , vk::ImageUsageFlags additionalUsage = vk::ImageUsageFlags()
            , Bool32 defaultImageView = VG_TRUE
            , Bool32 defaultSampler = VG_TRUE
            );
        ~TextureCubeColorAttachment();
        uint32_t getWidth() const;
        uint32_t getHeight() const;
        void applyData(const TextureDataInfo &layoutInfo
            , const void *memory
            , uint32_t size
            , Bool32 cacheMemory = VG_FALSE
            , Bool32 createMipmaps = VG_FALSE);
        virtual uint32_t getColorAttachmentWidth() const override;
        virtual uint32_t getColorAttachmentHeight() const override;
        virtual uint32_t getColorAttachmentLayers() const override;
        virtual const vk::Format getColorAttachmentFormat() const override;
        virtual const vk::ImageLayout getColorAttachmentLayout() const override;
        virtual const vk::ImageView *getColorAttachmentImageView() const override;
    private:
       TextureCubeColorAttachment() = delete;
       virtual void _init(Bool32 importContent) override;
    };

    class TextureCubeDepthStencilAttachment : public Texture, public BaseDepthStencilAttachment
    {
    public:
        TextureCubeDepthStencilAttachment(vk::Format format
            , uint32_t width
            , uint32_t height
            , Bool32 isInputUsage = VG_FALSE
            , vk::ImageUsageFlags additionalUsage = vk::ImageUsageFlags()
            , Bool32 defaultImageView = VG_TRUE
            , Bool32 defaultSampler = VG_TRUE
            );
        ~TextureCubeDepthStencilAttachment();
        uint32_t getWidth() const;
        uint32_t getHeight() const;
        void applyData(const TextureDataInfo &layoutInfo
            , const void *memory
            , uint32_t size
            , Bool32 cacheMemory = VG_FALSE
            , Bool32 createMipmaps = VG_FALSE);
        virtual uint32_t getDepthStencilAttachmentWidth() const override;
        virtual uint32_t getDepthStencilAttachmentHeight() const override;
        virtual uint32_t getDepthStencilAttachmentLayers() const override;
        virtual const vk::Format getDepthStencilAttachmentFormat() const override;
        virtual const vk::ImageLayout getDepthStencilAttachmentLayout() const override;
        virtual const vk::ImageView *getDepthStencilAttachmentImageView() const override;
    private:
       TextureCubeDepthStencilAttachment() = delete;
       virtual void _init(Bool32 importContent) override;
       void _checkDepthFormat();
    };

    class TextureCubeDepthAttachment : public Texture, public BaseDepthStencilAttachment
    {
    public:
        TextureCubeDepthAttachment(vk::Format format
            , uint32_t width
            , uint32_t height
            , Bool32 isInputUsage = VG_FALSE
            , vk::ImageUsageFlags additionalUsage = vk::ImageUsageFlags()
            , Bool32 defaultImageView = VG_TRUE
            , Bool32 defaultSampler = VG_TRUE
            );
        ~TextureCubeDepthAttachment();
        uint32_t getWidth() const;
        uint32_t getHeight() const;
        void applyData(const TextureDataInfo &layoutInfo
            , const void *memory
            , uint32_t size
            , Bool32 cacheMemory = VG_FALSE
            , Bool32 createMipmaps = VG_FALSE);
        virtual uint32_t getDepthStencilAttachmentWidth() const override;
        virtual uint32_t getDepthStencilAttachmentHeight() const override;
        virtual uint32_t getDepthStencilAttachmentLayers() const override;
        virtual const vk::Format getDepthStencilAttachmentFormat() const override;
        virtual const vk::ImageLayout getDepthStencilAttachmentLayout() const override;
        virtual const vk::ImageView *getDepthStencilAttachmentImageView() const override;
    private:
       TextureCubeDepthAttachment() = delete;
       virtual void _init(Bool32 importContent) override;
       void _checkDepthFormat();
    };
}

#endif // !VG_TEXTURE_CUBE_H
