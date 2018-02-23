#ifndef VG_TEXTURE_1D_ARRAY_H
#define VG_TEXTURE_1D_ARRAY_H

#include "graphics/texture/texture.hpp"

namespace vg
{
	class Texture1DArray : public Texture
	{
	public:
		Texture1DArray(TextureFormat format, Bool32 mipmap, uint32_t width, uint32_t arraylength);
		~Texture1DArray();
		uint32_t getWidth() const;
		uint32_t getArrayLength() const;
		std::vector<Color> getPixels(uint32_t arrayIndex, uint32_t mipLevel = 0) const;
		std::vector<Color32> getPixels32(uint32_t arrayIndex, uint32_t mipLevel = 0) const;
		void setPixels(const std::vector<Color> &colors, uint32_t arrayIndex, uint32_t mipLevel = 0);
		void setPixels32(const std::vector<Color32> &colors, uint32_t arrayIndex, uint32_t mipLevel = 0);
		void setPixels(const void* colors, uint32_t size, uint32_t arrayIndex, uint32_t mipLevel = 0);
		void setPixels32(const void* colors, uint32_t size, uint32_t arrayIndex, uint32_t mipLevel = 0);
		void apply(Bool32 updateMipmaps = VG_TRUE, Bool32 makeUnreadable = VG_FALSE);
	private:
		Texture1DArray() = delete;
	};
}

#endif // !VG_TEXTURE_1D_ARRAY_H
