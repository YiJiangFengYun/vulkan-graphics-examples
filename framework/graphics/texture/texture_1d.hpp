#ifndef VG_TEXTURE_1D_H
#define VG_TEXTURE_1D_H

#include "graphics/texture/texture.hpp"

namespace vg
{
	class Texture1D : public Texture
	{
	public:
		Texture1D(TextureFormat format, Bool32 mipmap, uint32_t width);
		~Texture1D();
		uint32_t getWidth() const;
		void applyData(TextureDataLayout layoutInfo
			, const void *memory
			, uint32_t size
			, Bool32 cacheMemory = VG_FALSE
		    , Bool32 createMipmaps = VG_FALSE);
	private:
		Texture1D() = delete;
	};
}

#endif // !VG_TEXTURE_1D_H
