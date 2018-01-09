#include "graphics/texture/texture_1d_array.hpp"

namespace kgs
{
	Texture1DArray::Texture1DArray(TextureFormat format, Bool32 mipMap, uint32_t width, uint32_t arraylength)
		:Texture(format, mipMap)
	{
		m_type = TextureType::TEX_1D_ARRAY;
		m_width = width;
		m_arrayLength = arraylength;
		_init();
	}

	Texture1DArray::~Texture1DArray()
	{
	}

	uint32_t Texture1DArray::getWidth()
	{
		return m_width;
	}

	uint32_t Texture1DArray::getArrayLength()
	{
		return m_arrayLength;
	}

	std::vector<Color> Texture1DArray::getPixels(uint32_t arrayIndex, uint32_t mipLevel)
	{
		return _getPixels(arrayIndex, mipLevel);
	}

	std::vector<Color32> Texture1DArray::getPixels32(uint32_t arrayIndex, uint32_t mipLevel)
	{
		return _getPixels32(arrayIndex, mipLevel);
	}

	void Texture1DArray::setPixels(std::vector<Color> colors, uint32_t arrayIndex, uint32_t mipLevel)
	{
		_setPixels(colors, arrayIndex, mipLevel);
	}

	void Texture1DArray::setPixels32(std::vector<Color32> colors, uint32_t arrayIndex, uint32_t mipLevel)
	{
		_setPixels32(colors, arrayIndex, mipLevel);
	}

	void Texture1DArray::apply(Bool32 updateMipmaps, Bool32 makeUnreadable)
	{
		_apply(updateMipmaps, makeUnreadable);
	}
}