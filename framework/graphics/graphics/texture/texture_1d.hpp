#ifndef VG_TEXTURE_1D_H
#define VG_TEXTURE_1D_H

#include "graphics/texture/texture.hpp"

namespace vg
{
	class Texture1D : public Texture
	{
	public:
		Texture1D(vk::Format format
		    , Bool32 mipmap
			, uint32_t width
			, vk::ImageUsageFlags additionalUsage = vk::ImageUsageFlags()
			, Bool32 defaultImageView = VG_TRUE
			, Bool32 defaultSampler = VG_TRUE
			);
		~Texture1D();
		uint32_t getWidth() const;
		void applyData(const TextureDataInfo &layoutInfo
			, const void *memory
			, uint32_t size
			, Bool32 cacheMemory = VG_FALSE
		    , Bool32 createMipmaps = VG_FALSE);
	private:
		Texture1D() = delete;
	};
}

#endif // !VG_TEXTURE_1D_H
