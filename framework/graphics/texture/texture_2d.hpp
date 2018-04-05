#ifndef VG_TEXTURE_2D_H
#define VG_TEXTURE_2D_H

#include "graphics/texture/texture.hpp"
#include "graphics/texture/attachment.hpp"

namespace vg
{
	class Texture2D : public Texture
	{
	public:
		Texture2D(vk::Format format, Bool32 mipmap, uint32_t width, uint32_t height);
		~Texture2D();
		uint32_t getWidth() const;
		uint32_t getHeight() const;
		void applyData(const TextureDataInfo &layoutInfo
			, const void *memory
			, uint32_t size
			, Bool32 cacheMemory = VG_FALSE
		    , Bool32 createMipmaps = VG_FALSE);
	private:
		Texture2D() = delete;
	};

	class Texture2DColorAttachment : public Texture, public BaseColorAttachment
	{
	public:
	    Texture2DColorAttachment(vk::Format format, Bool32 mipmap, uint32_t width, uint32_t height);
		~Texture2DColorAttachment();
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
		virtual vk::Format getColorAttachmentFormat() const override;
		virtual vk::ImageLayout getColorAttachmentLayout() const override;
		virtual vk::ImageView *getColorAttachmentImageView() const override;
	private:
	   Texture2DColorAttachment() = delete;
	};

	class Texture2DDepthStencilAttachment : public Texture, public BaseDepthStencilAttachment
	{
	public:
	    Texture2DDepthStencilAttachment(vk::Format format, Bool32 mipmap, uint32_t width, uint32_t height);
		~Texture2DDepthStencilAttachment();
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
		virtual vk::Format getDepthStencilAttachmentFormat() const override;
		virtual vk::ImageLayout getDepthStencilAttachmentLayout() const = 0;
		virtual vk::ImageView *getDepthStencilAttachmentImageView() const = 0;
	private:
	   Texture2DDepthStencilAttachment() = delete;
	   void _checkDepthFormat();
	};
}

#endif // !VG_TEXTURE_2D_H
