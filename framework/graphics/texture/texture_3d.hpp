#ifndef KGS_TEXTURE_3D_H
#define KGS_TEXTURE_3D_H

#include "graphics/texture/texture.hpp"

namespace kgs
{
	class Texture3D : public Texture
	{
	public:
		Texture3D(TextureFormat format, Bool32 mipmap, uint32_t width, uint32_t height, uint32_t depth);
		~Texture3D();
		uint32_t getWidth();
		uint32_t getHeight();
		uint32_t getDepth();
		std::vector<Color> getPixels(uint32_t mipLevel = 0);
		std::vector<Color32> getPixels32(uint32_t mipLevel = 0);
		void setPixels(std::vector<Color> colors, uint32_t mipLevel = 0);
		void setPixels32(std::vector<Color32> colors, uint32_t mipLevel = 0);
		void apply(Bool32 updateMipmaps = KGS_TRUE, Bool32 makeUnreadable = KGS_FALSE);
	private:
		Texture3D() = delete;
	};
}

#endif // !KGS_TEXTURE_3D_H
