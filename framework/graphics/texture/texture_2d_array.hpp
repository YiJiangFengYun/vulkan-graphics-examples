#ifndef VG_TEXTURE_2D_ARRAY_H
#define VG_TEXTURE_2D_ARRAY_H

#include "graphics/texture/texture.hpp"

namespace vg
{
	class Texture2DArray : public Texture
	{
	public:
		Texture2DArray(vk::Format format
		    , Bool32 mipmap
			, uint32_t width
			, uint32_t height
			, uint32_t arraylength
			, Bool32 defaultImageView = VG_TRUE
			, Bool32 defaultSampler = VG_TRUE
			);
		~Texture2DArray();
		uint32_t getWidth() const;
		uint32_t getHeight() const;
		uint32_t getArrayLength() const;
		void applyData(const TextureDataInfo &layoutInfo
			, const void *memory
			, uint32_t size
			, Bool32 cacheMemory = VG_FALSE
		    , Bool32 createMipmaps = VG_FALSE);
	private:
		Texture2DArray() = delete;
	};
}

#endif // !VG_TEXTURE_2D_ARRAY_H
