#ifndef KGS_TEXTURE_H
#define KGS_TEXTURE_H

#include <vulkan/vulkan.hpp>

namespace kgs
{
	class Texture
	{
	public:
		Texture();
		~Texture();

	protected:
		vk::ImageCreateInfo m_imageCreateInfo;
		vk::ImageViewCreateInfo m_imageViewCreateInfo;
		vk::SamplerCreateInfo m_samplerCreateInfo;
		vk::Image m_image;
		vk::ImageView m_imageView;
		vk::Sampler m_sampler;
		
		void _createImage();
		void _destroyImage();
		void _createImageView();
		void _destroyImageView();
		void _createSampler();
		void _destroySampler();
	private:
		
	};
}

#endif // !KGS_TEXTURE_H
