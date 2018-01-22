#ifndef VG_TEXTURE_CUBE_ARRAY_H
#define VG_TEXTURE_CUBE_ARRAY_H

#include "graphics/texture/texture.hpp"

namespace vg
{
	class TextureCubeArray : public Texture
	{
	public:
		TextureCubeArray(TextureFormat format, Bool32 mipmap, uint32_t size, uint32_t arraylength);
		~TextureCubeArray();
		uint32_t getSize();
		uint32_t getArrayLength();
		std::vector<Color> getPixels(CubemapFace face, uint32_t arrayIndex, uint32_t mipLevel = 0);
		std::vector<Color32> getPixels32(CubemapFace face, uint32_t arrayIndex, uint32_t mipLevel = 0);
		void setPixels(std::vector<Color> colors, CubemapFace face, uint32_t arrayIndex, uint32_t mipLevel = 0);
		void setPixels32(std::vector<Color32> colors, CubemapFace face, uint32_t arrayIndex, uint32_t mipLevel = 0);
		void apply(Bool32 updateMipmaps = VG_TRUE, Bool32 makeUnreadable = VG_FALSE);
	private:
		TextureCubeArray() = delete;
	};
}

#endif // !VG_TEXTURE_CUBE_ARRAY_H
