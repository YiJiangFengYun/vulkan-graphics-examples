#include "graphics/texture/texture_2d_array.hpp"

namespace vg
{
	Texture2DArray::Texture2DArray(TextureFormat format, Bool32 mipMap, uint32_t width, uint32_t height, uint32_t arraylength)
		:Texture(format, mipMap)
	{
		m_type = TextureType::TEX_2D_ARRAY;
		m_width = width;
		m_height = height;
		m_arrayLength = arraylength;
		_init();
	}

	Texture2DArray::~Texture2DArray()
	{
	}

	uint32_t Texture2DArray::getWidth() const
	{
		return m_width;
	}

	uint32_t Texture2DArray::getHeight() const
	{
		return m_height;
	}

	uint32_t Texture2DArray::getArrayLength() const
	{
		return m_arrayLength;
	}

	std::vector<Color> Texture2DArray::getPixels(uint32_t arrayIndex, uint32_t mipLevel) const
	{
		return _getPixels(arrayIndex, mipLevel);
	}

	std::vector<Color32> Texture2DArray::getPixels32(uint32_t arrayIndex, uint32_t mipLevel) const
	{
		return _getPixels32(arrayIndex, mipLevel);
	}

	void Texture2DArray::setPixels(const std::vector<Color> &colors, uint32_t arrayIndex, uint32_t mipLevel)
	{
		_setPixels(colors, arrayIndex, mipLevel);
	}

	void Texture2DArray::setPixels32(const std::vector<Color32> &colors, uint32_t arrayIndex, uint32_t mipLevel)
	{
		_setPixels32(colors, arrayIndex, mipLevel);
	}

	void Texture2DArray::setPixels(const void* colors, uint32_t size, uint32_t arrayIndex, uint32_t mipLevel)
	{
		_setPixels(colors, size, arrayIndex, mipLevel);
	}

	void Texture2DArray::setPixels32(const void* colors, uint32_t size, uint32_t arrayIndex, uint32_t mipLevel)
	{
		_setPixels32(colors, size, arrayIndex, mipLevel);
	}

	void Texture2DArray::apply(Bool32 updateMipmaps, Bool32 makeUnreadable)
	{
		_apply(updateMipmaps, makeUnreadable);
	}
}