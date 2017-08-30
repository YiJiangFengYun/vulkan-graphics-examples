#ifndef KGS_TEXTURE_H
#define KGS_TEXTURE_H

#include <vulkan/vulkan.hpp>
#include "graphics/global.hpp"
#include "graphics/texture/texture_option.hpp"
#include "graphics/device/device.hpp"

namespace kgs
{
	class Texture
	{
	public:
		Texture(Device device, TextureFormat format, Bool32 mipMap);
		~Texture();

	protected:
		//--compositions
		TextureType m_type;
		TextureFormat m_format;
		FilterMode m_filterMode;
		float m_anisotropy;
		Bool32 m_mipMap;
		SamplerAddressMode m_samplerAddressMode;

		//--aggregations
		Device m_device;
		vk::PhysicalDevice m_physicalDevice;
		vk::Device m_nativeDevice;

		vk::ImageCreateInfo m_imageCreateInfo;
		vk::ImageViewCreateInfo m_imageViewCreateInfo;
		vk::SamplerCreateInfo m_samplerCreateInfo;
		vk::Image m_image;
		vk::DeviceMemory m_memory;
		vk::ImageView m_imageView;
		vk::Sampler m_sampler;
		
		void _createImage(TextureType type, TextureFormat format, uint32_t width, uint32_t height, uint32_t depth, uint32_t arrayLength);
		void _destroyImage();
		void _createImageView(TextureType type, TextureFormat format, int32_t arrayLength);
		void _destroyImageView();
		void _createSampler(FilterMode filterMode, SamplerAddressMode samplerAddressMode, float anisotropy);
		void _destroySampler();
	private:

		//tool methods
		uint32_t _findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);
	};
}

#endif // !KGS_TEXTURE_H
