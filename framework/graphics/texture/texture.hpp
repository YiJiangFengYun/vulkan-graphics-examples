#ifndef VG_TEXTURE_H
#define VG_TEXTURE_H

#include "foundation/wrapper.hpp"
#include "graphics/util/find_memory.hpp"
#include "graphics/global.hpp"
#include "graphics/module.hpp"
#include "graphics/util/single_time_command.hpp"
#include "graphics/texture/texture_option.hpp"

namespace vg
{
	struct TextureDataLayout
	{
		struct Component {
			uint32_t mipLevel;
			uint32_t baseArrayLayer;
			uint32_t layerCount;
			uint32_t size;
			Bool32 hasImageExtent = VG_FALSE;
			uint32_t width = 0u;
			uint32_t height = 0u;
			uint32_t depth = 0u;
			Component();
			Component(uint32_t mipLevel
				, uint32_t baseArrayLayer
				, uint32_t layerCount
				, uint32_t size
				, Bool32 hasImageExtent
				, uint32_t width
				, uint32_t height
				, uint32_t depth
			);
		};
		uint32_t componentCount;
		Component *pComponent;

		TextureDataLayout();
	};

	class Texture : public Base
	{
	public:
		Texture(TextureFormat format, Bool32 mipMap);
		~Texture();
		float getAnisotropy() const;
		void setAnisotropy(float value);
		FilterMode getFilterMode() const;
		void setFilterMode(FilterMode value);
		SamplerAddressMode getSamplerAddressMode() const;
		void setSamplerAddressMode(SamplerAddressMode value);

		TextureType getType() const;
		TextureFormat getFormat() const;
		Bool32 getIsMipmap() const;
		uint32_t getMipmapLevels() const;
		uint32_t getArrayLayerCount() const;

		vk::Format getVKFormat() const;
		vk::ImageLayout getImageLayout() const;
		std::shared_ptr<vk::Image> getImage() const;
		std::shared_ptr<vk::DeviceMemory> getImageMemory() const;
		std::shared_ptr<vk::ImageView> getImageView() const;
		std::shared_ptr<vk::Sampler> getSampler() const;
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
		vk::ImageUsageFlags m_vkImageUsageFlags;
		vk::ImageLayout m_vkImageLayout;
		vk::ImageAspectFlags m_vkImageAspectFlags;

		uint32_t m_mipMapLevels;
		uint32_t m_arrayLayer;
		vk::Format m_vkFormat;
		vk::ImageLayout m_currVkImageLayout; //record texture current image layout state.
		vk::Filter m_vkFilter;
		vk::SamplerMipmapMode m_vkSamplerMipmapMode;
		vk::SamplerAddressMode m_vkSamplerAddressMode;

		//optional storage
		TextureDataLayout m_dataLayout;
		std::vector<TextureDataLayout::Component> m_components;
		void *m_pMemory;
		uint32_t m_memorySize;
		uint32_t m_realSize;

		//--aggregations

		std::shared_ptr<vk::Image> m_pImage;
		std::shared_ptr<vk::DeviceMemory> m_pImageMemory;
		std::shared_ptr<vk::ImageView> m_pImageView;
		std::shared_ptr<vk::Sampler> m_pSampler;
		
		virtual void _init();

		void _updateMipMapLevels();
		void _updateArrayLayer();
		void _updateVkFormat();
		void _updateVkFilter();
		void _updateVkSamplerAddressMode();
		void _createImage();
		void _createImageView();
		void _createSampler();

		void _applyData(const TextureDataLayout &layoutInfo
			, const void *memory
			, uint32_t size
			, Bool32 cacheMemory = VG_FALSE
		    , Bool32 createMipmaps = VG_FALSE);

		void _createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties,
			std::shared_ptr<vk::Buffer>& pBuffer, std::shared_ptr<vk::DeviceMemory>& pBufferMemory);
		void _tranImageLayout(std::shared_ptr<vk::CommandBuffer> pCommandBuffer, vk::Image image,
			vk::ImageLayout oldLayout, vk::ImageLayout newLayout,
			uint32_t baseMipLevel, uint32_t levelCount,
			uint32_t baseArrayLayer, uint32_t layerCount);
		void _copyBufferToImage(std::shared_ptr<vk::CommandBuffer> pCommandBuffer, vk::Buffer buffer, vk::Image image,
			uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevel,
			uint32_t baseArrayLayer, uint32_t layerCount);
	};
}

#endif // !VG_TEXTURE_H
