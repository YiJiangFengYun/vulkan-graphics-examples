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
		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_depth;
		uint32_t m_arrayLength;
		TextureType m_type;
		TextureFormat m_format;
		FilterMode m_filterMode;
		float m_anisotropy;
		Bool32 m_mipMap;
		SamplerAddressMode m_samplerAddressMode;

		uint32_t m_mipMapLevels;
		uint32_t m_arrayLayer;
		vk::Format m_vkFormat;
		vk::ImageLayout m_vkImageLayout; //recode texture current image layout state.

		//temp
	    std::vector<std::vector<Color32>> arrTempColors;

		//--aggregations
		Device m_device;

		vk::ImageCreateInfo m_imageCreateInfo;
		vk::ImageViewCreateInfo m_imageViewCreateInfo;
		vk::SamplerCreateInfo m_samplerCreateInfo;
		vk::Image m_image;
		vk::DeviceMemory m_memory;
		vk::ImageView m_imageView;
		vk::Sampler m_sampler;
		
		void _caculateMipMapLevels();
		void _caculateArrayLayer();
		void _caculateVkFormat();
		void _createImage();
		void _destroyImage();
		void _createImageView();
		void _destroyImageView();
		void _createSampler(FilterMode filterMode, SamplerAddressMode samplerAddressMode, float anisotropy);
		void _destroySampler();

		std::vector<Color> _getPixels(uint32_t layer, uint32_t mipLevel = 0);
		std::vector<Color32> _getPixels32(uint32_t layer, uint32_t mipLevel = 0);
		void _setPixels(std::vector<Color> colors, uint32_t layer, uint32_t mipLevel = 0);
		void _setPixels32(std::vector<Color32> colors, uint32_t layer, uint32_t mipLevel = 0);
		void _apply(Bool32 updateMipmaps = KGS_TRUE, Bool32 makeUnreadable = KGS_FALSE);
	private:

		void _createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties,
			vk::Buffer& buffer, vk::DeviceMemory& bufferMemory);

		void  _tranImageLayout(vk::CommandBuffer commandBuffer, vk::Image image,
			vk::ImageLayout oldLayout, vk::ImageLayout newLayout,
			uint32_t baseMipLevel, uint32_t levelCount,
			uint32_t baseArrayLayer, uint32_t layerCount);
		void _copyBufferToImage(vk::CommandBuffer commandBuffer, vk::Buffer buffer, vk::Image image,
			uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevel,
			uint32_t baseArrayLayer, uint32_t layerCount);
		void _resizeColorsData(uint32_t mipLevel);

		//tool methods
		uint32_t _findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);
		vk::CommandBuffer _beginSingleTimeCommands();
		void _endSingleTimeCommands(vk::CommandBuffer commandBuffer);
	};
}

#endif // !KGS_TEXTURE_H
