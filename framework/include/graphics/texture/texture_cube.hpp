#ifndef KGS_TEXTURE_CUBE_H
#define KGS_TEXTURE_CUBE_H

#include "graphics/texture/texture.hpp"

namespace kgs
{
	class TextureCube : public Texture
	{
	public:
		TextureCube(TextureFormat format, Bool32 mipmap, uint32_t size);
		~TextureCube();
		std::vector<Color> getPixels(CubemapFace face, uint32_t mipLevel = 0);
		std::vector<Color32> getPixels32(CubemapFace face, uint32_t mipLevel = 0);
		void setPixels(std::vector<Color> colors, CubemapFace face, uint32_t mipLevel = 0);
		void setPixels32(std::vector<Color32> colors, CubemapFace face, uint32_t mipLevel = 0);
		void apply(Bool32 updateMipmaps = KGS_TRUE, Bool32 makeUnreadable = KGS_FALSE);
	private:
		TextureCube() = delete;
	};
}

#endif // !KGS_TEXTURE_CUBE_H
