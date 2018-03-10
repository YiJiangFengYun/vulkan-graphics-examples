#ifndef VG_TEXTURE_CUBE_H
#define VG_TEXTURE_CUBE_H

#include "graphics/texture/texture.hpp"

namespace vg
{
	class TextureCube : public Texture
	{
	public:
		TextureCube(TextureFormat format, Bool32 mipmap, uint32_t size);
		~TextureCube();
		uint32_t getSize() const;
		void applyData(TextureDataLayout layoutInfo
			, const void *memory
			, uint32_t size
			, Bool32 cacheMemory = VG_FALSE
		    , Bool32 createMipmaps = VG_FALSE);
	private:
		TextureCube() = delete;
	};
}

#endif // !VG_TEXTURE_CUBE_H
