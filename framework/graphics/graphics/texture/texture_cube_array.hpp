#ifndef VG_TEXTURE_CUBE_ARRAY_H
#define VG_TEXTURE_CUBE_ARRAY_H

#include "graphics/texture/texture.hpp"

namespace vg
{
	class TextureCubeArray : public Texture
	{
	public:
		TextureCubeArray(vk::Format format
		    , Bool32 mipmap
			, uint32_t width
			, uint32_t height
			, uint32_t arraylength
			, vk::ImageUsageFlags additionalUsage = vk::ImageUsageFlags()
			, Bool32 defaultImageView = VG_TRUE
			, Bool32 defaultSampler = VG_TRUE
			);
		~TextureCubeArray();
		uint32_t getSize() const;
		uint32_t getArrayLength() const;
		void applyData(const TextureDataInfo &layoutInfo
			, const void *memory
			, uint32_t size
			, Bool32 cacheMemory = VG_FALSE
		    , Bool32 createMipmaps = VG_FALSE);
	private:
		TextureCubeArray() = delete;
	};
}

#endif // !VG_TEXTURE_CUBE_ARRAY_H
