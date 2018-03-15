#ifndef VG_TEXTURE_2D_H
#define VG_TEXTURE_2D_H

#include "graphics/texture/texture.hpp"

namespace vg
{
	class Texture2D : public Texture
	{
	public:
		Texture2D(vk::Format format, Bool32 mipmap, uint32_t width, uint32_t height);
		~Texture2D();
		uint32_t getWidth() const;
		uint32_t getHeight() const;
		void applyData(const TextureDataLayout &layoutInfo
			, const void *memory
			, uint32_t size
			, Bool32 cacheMemory = VG_FALSE
		    , Bool32 createMipmaps = VG_FALSE);
	private:
		Texture2D() = delete;
	};
}

#endif // !VG_TEXTURE_2D_H
