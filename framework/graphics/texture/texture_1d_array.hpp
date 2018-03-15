#ifndef VG_TEXTURE_1D_ARRAY_H
#define VG_TEXTURE_1D_ARRAY_H

#include "graphics/texture/texture.hpp"

namespace vg
{
	class Texture1DArray : public Texture
	{
	public:
		Texture1DArray(vk::Format format, Bool32 mipmap, uint32_t width, uint32_t arraylength);
		~Texture1DArray();
		uint32_t getWidth() const;
		uint32_t getArrayLength() const;
		void applyData(const TextureDataLayout &layoutInfo
			, const void *memory
			, uint32_t size
			, Bool32 cacheMemory = VG_FALSE
		    , Bool32 createMipmaps = VG_FALSE);
	private:
		Texture1DArray() = delete;
	};
}

#endif // !VG_TEXTURE_1D_ARRAY_H
