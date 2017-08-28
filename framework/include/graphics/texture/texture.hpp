#ifndef KGS_TEXTURE_H
#define KGS_TEXTURE_H

#include <vulkan/vulkan.hpp>
#include "graphics/texture/texture_option.hpp"
#include "graphics/device/device.hpp"

namespace kgs
{
	class Texture
	{
	public:
		Texture(Device device);
		~Texture();

	protected:
		//--compositions
		TextureType m_type;
		FilterMode m_filterMode;
		float m_anisotropy;
		SamplerAddressMode m_samplerAddressMode;

		//--aggregations
		Device m_device;
		vk::Device m_nativeDevice;

		vk::ImageCreateInfo m_imageCreateInfo;
		vk::ImageViewCreateInfo m_imageViewCreateInfo;
		vk::SamplerCreateInfo m_samplerCreateInfo;
		vk::Image m_image;
		vk::DeviceMemory m_memory;
		vk::ImageView m_imageView;
		vk::Sampler m_sampler;
		
		virtual void _createImage() = 0;
		virtual void _destroyImage();
		virtual void _createImageView() = 0;
		virtual void _destroyImageView();
		virtual void _createSampler() = 0;
		virtual void _destroySampler();
	private:
	};
}

#endif // !KGS_TEXTURE_H
