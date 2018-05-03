#ifndef VG_TEXTURE_CUBE_H
#define VG_TEXTURE_CUBE_H

#include "graphics/texture/texture.hpp"

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
}

#endif // !VG_TEXTURE_CUBE_H
