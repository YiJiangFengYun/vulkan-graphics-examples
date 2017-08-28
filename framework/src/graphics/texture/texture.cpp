#include "graphics/texture/texture.hpp"

namespace kgs
{
	Texture::Texture(Device device)
		:m_device(device), 
		m_nativeDevice(device.getNativeDevice())
	{
	}

	Texture::~Texture()
	{
		_destroySampler();
		_destroyImageView();
		_destroyImage();
	}

	void Texture::_destroyImage()
	{
		m_nativeDevice.destroyImage(m_image);
		m_nativeDevice.freeMemory(m_memory);
	}

	void Texture::_destroyImageView()
	{
		m_nativeDevice.destroyImageView(m_imageView);
	}

	void Texture::_destroySampler()
	{
		m_nativeDevice.destroySampler(m_sampler);
	}
}