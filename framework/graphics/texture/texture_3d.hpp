#ifndef VG_TEXTURE_3D_H
#define VG_TEXTURE_3D_H

#include "graphics/texture/texture.hpp"

namespace vg
{
	class Texture3D : public Texture
	{
	public:
		Texture3D(TextureFormat format, Bool32 mipmap, uint32_t width, uint32_t height, uint32_t depth);
		~Texture3D();
		uint32_t getWidth() const;
		uint32_t getHeight() const;
		uint32_t getDepth() const;
		std::vector<Color> getPixels(uint32_t mipLevel = 0) const;
		std::vector<Color32> getPixels32(uint32_t mipLevel = 0) const;
		void setPixels(const std::vector<Color> &colors, uint32_t mipLevel = 0);
		void setPixels32(const std::vector<Color32> &colors, uint32_t mipLevel = 0);
		void setPixels(const void* colors, uint32_t size, uint32_t mipLevel = 0);
		void setPixels32(const void* colors, uint32_t size, uint32_t mipLevel = 0);
		void apply(Bool32 updateMipmaps = VG_TRUE, Bool32 makeUnreadable = VG_FALSE);
	private:
		Texture3D() = delete;
	};
}

#endif // !VG_TEXTURE_3D_H
