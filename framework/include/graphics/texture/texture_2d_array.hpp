#ifndef KGS_TEXTURE_2D_ARRAY_H
#define KGS_TEXTURE_2D_ARRAY_H

#include "graphics/texture/texture.hpp"

#include <glm/glm.hpp>
#include "graphics/global.hpp"

namespace kgs
{
	class Texture2DArray : public Texture
	{
	public:
		Texture2DArray(TextureFormat format, Bool32 mipmap, uint32_t width, uint32_t height, uint32_t arraylength);
		~Texture2DArray();
		std::vector<Color> getPixels(uint32_t arrayIndex, uint32_t mipLevel = 0);
		std::vector<Color32> getPixels32(uint32_t arrayIndex, uint32_t mipLevel = 0);
		void setPixels(std::vector<Color> colors, uint32_t arrayIndex, uint32_t mipLevel = 0);
		void setPixels32(std::vector<Color32> colors, uint32_t arrayIndex, uint32_t mipLevel = 0);
		void apply(Bool32 updateMipmaps = KGS_TRUE, Bool32 makeUnreadable = KGS_FALSE);
	private:
		Texture2DArray() = delete;
	};
}

#endif // !KGS_TEXTURE_2D_ARRAY_H
