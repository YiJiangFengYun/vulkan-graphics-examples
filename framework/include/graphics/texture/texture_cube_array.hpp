#ifndef KGS_TEXTURE_CUBE_ARRAY_H
#define KGS_TEXTURE_CUBE_ARRAY_H

#include "graphics/texture/texture.hpp"

namespace kgs
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
		void apply(Bool32 updateMipmaps = KGS_TRUE, Bool32 makeUnreadable = KGS_FALSE);
	private:
		TextureCubeArray() = delete;
	};
}

#endif // !KGS_TEXTURE_CUBE_ARRAY_H
