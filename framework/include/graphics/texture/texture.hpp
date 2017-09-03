#ifndef KGS_TEXTURE_H
#define KGS_TEXTURE_H

#include <vulkan/vulkan.hpp>
#include "foundation/wrapper.hpp"
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
		vk::Filter m_vkFilter;
		vk::SamplerMipmapMode m_vkSamplerMipmapMode;
		vk::SamplerAddressMode m_vkSamplerAddressMode;

		//temp
	    std::vector<std::vector<Color32>> m_arrTempColors;

		//--aggregations
		Device m_device;

		std::shared_ptr<vk::Image> m_pImage;
		std::shared_ptr<vk::DeviceMemory> m_pMemory;
		std::shared_ptr<vk::ImageView> m_pImageView;
		std::shared_ptr<vk::Sampler> m_pSampler;
		
		void _updateMipMapLevels();
		void _updateArrayLayer();
		void _updateVkFormat();
		void _updateVkFilter();
		void _updateVkSamplerAddressMode();
		void _createImage();
		void _createImageView();
		void _createSampler();

		std::vector<Color> _getPixels(uint32_t layer, uint32_t mipLevel = 0);
		std::vector<Color32> _getPixels32(uint32_t layer, uint32_t mipLevel = 0);
		void _setPixels(std::vector<Color> colors, uint32_t layer, uint32_t mipLevel = 0);
		void _setPixels32(std::vector<Color32> colors, uint32_t layer, uint32_t mipLevel = 0);
		void _apply(Bool32 updateMipmaps = KGS_TRUE, Bool32 makeUnreadable = KGS_FALSE);
	private:
		void _applyWithGenMipMap();
		void _applyDirect();

		void _createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties,
			std::shared_ptr<vk::Buffer>& pBuffer, std::shared_ptr<vk::DeviceMemory>& pBufferMemory);
		void  _tranImageLayout(std::shared_ptr<vk::CommandBuffer> pCommandBuffer, vk::Image image,
			vk::ImageLayout oldLayout, vk::ImageLayout newLayout,
			uint32_t baseMipLevel, uint32_t levelCount,
			uint32_t baseArrayLayer, uint32_t layerCount);
		void _copyBufferToImage(std::shared_ptr<vk::CommandBuffer> pCommandBuffer, vk::Buffer buffer, vk::Image image,
			uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevel,
			uint32_t baseArrayLayer, uint32_t layerCount);
		void _resizeColorsData(uint32_t mipLevel);

		//tool methods
		uint32_t _findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);
		std::shared_ptr<vk::CommandBuffer> _beginSingleTimeCommands();
		void _endSingleTimeCommands(std::shared_ptr<vk::CommandBuffer> pCommandBuffer);
	};
}

#endif // !KGS_TEXTURE_H
