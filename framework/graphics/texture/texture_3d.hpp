#ifndef VG_TEXTURE_3D_H
#define VG_TEXTURE_3D_H

#include "graphics/texture/texture.hpp"

namespace vg
{
	class Texture3D : public Texture
	{
	public:
		Texture3D(vk::Format format, Bool32 mipmap, uint32_t width, uint32_t height, uint32_t depth);
		~Texture3D();
		uint32_t getWidth() const;
		uint32_t getHeight() const;
		uint32_t getDepth() const;
		void applyData(const TextureDataInfo &layoutInfo
			, const void *memory
			, uint32_t size
			, Bool32 cacheMemory = VG_FALSE
		    , Bool32 createMipmaps = VG_FALSE);
	private:
		Texture3D() = delete;
	};
}

#endif // !VG_TEXTURE_3D_H
