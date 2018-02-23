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
		std::vector<Color> getPixels(CubemapFace face, uint32_t mipLevel = 0) const;
		std::vector<Color32> getPixels32(CubemapFace face, uint32_t mipLevel = 0) const;
		void setPixels(const std::vector<Color> &colors, CubemapFace face, uint32_t mipLevel = 0);
		void setPixels32(const std::vector<Color32> &colors, CubemapFace face, uint32_t mipLevel = 0);
		void setPixels(const void* colors, uint32_t size, CubemapFace face, uint32_t mipLevel = 0);
		void setPixels32(const void* colors, uint32_t size, CubemapFace face, uint32_t mipLevel = 0);
		void apply(Bool32 updateMipmaps = VG_TRUE, Bool32 makeUnreadable = VG_FALSE);
	private:
		TextureCube() = delete;
	};
}

#endif // !VG_TEXTURE_CUBE_H
