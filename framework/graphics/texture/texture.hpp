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
	struct TextureDataInfo
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

		TextureDataInfo();
	};

	class Texture : public Base
	{
	public:
	    struct ImageInfo {
			vk::ImageCreateFlags flags;
		    vk::ImageType imageType;
		    vk::Format format;			
		    vk::Extent3D extent;
		    uint32_t mipLevels;			
		    uint32_t arrayLayers;
		    vk::SampleCountFlagBits samples;
		    vk::ImageTiling tiling;
		    vk::ImageUsageFlags usage;
		    vk::SharingMode sharingMode;
		    vk::ImageLayout layout;
			vk::ImageAspectFlags allAspect;

			ImageInfo(vk::ImageCreateFlags flags = vk::ImageCreateFlags()
				, vk::ImageType imageType = vk::ImageType()
			    , vk::Format format = vk::Format()
			    , vk::Extent3D extent = vk::Extent3D()
			    , uint32_t mipLevels = 1u
			    , uint32_t arrayLayers = 1u
                , vk::SampleCountFlagBits samples = vk::SampleCountFlagBits()
			    , vk::ImageTiling tiling = vk::ImageTiling()
			    , vk::ImageUsageFlags usage = vk::ImageUsageFlags()
			    , vk::SharingMode sharingMode = vk::SharingMode()
			    , vk::ImageLayout layout = vk::ImageLayout()
				, vk::ImageAspectFlags allAspect = vk::ImageAspectFlags()
				);			
		};


	    struct Image {
		public:
		    Image(ImageInfo info);
			ImageInfo getInfo() const;
			const vk::Image *getImage() const;
			const vk::DeviceMemory *getImageMemory() const;

		private:
		    Image() = delete;
		    ImageInfo m_info;
		    std::shared_ptr<vk::Image> m_pImage;
		    std::shared_ptr<vk::DeviceMemory> m_pImageMemory;
			void _create();
		};

		struct ImageViewInfo {
			vk::ImageViewCreateFlags flags;
			vk::Image image;
			vk::ImageViewType viewType;
			vk::Format format;
			vk::ComponentMapping components;
			vk::ImageSubresourceRange subResourceRange;

			ImageViewInfo(vk::ImageViewCreateFlags flags = vk::ImageViewCreateFlags()
			    , vk::Image image = vk::Image()
			    , vk::ImageViewType viewType = vk::ImageViewType()
			    , vk::Format format = vk::Format()
			    , vk::ComponentMapping components = vk::ComponentMapping()
        		, vk::ImageSubresourceRange subResourceRange = vk::ImageSubresourceRange()
				);
		};

		struct ImageViewCreateInfo {
			vk::ComponentMapping components;
			vk::ImageSubresourceRange subResourceRange;

			ImageViewCreateInfo(vk::ComponentMapping components = vk::ComponentMapping()
        		, vk::ImageSubresourceRange subResourceRange = vk::ImageSubresourceRange()
				);
		};

		struct ImageView {
		public:
		    ImageView(ImageViewInfo info);
			ImageViewInfo getInfo() const;
			const vk::ImageView *getImageView() const;
		private:
		    ImageView() = delete;
			ImageViewInfo m_info;
		    std::shared_ptr<vk::ImageView> m_pImageView;
			void _create();
		};

		struct SamplerInfo {
			vk::SamplerCreateFlags flags;
			vk::Filter magFilter;
			vk::Filter minFilter;
			vk::SamplerMipmapMode mipmapMode;
			vk::SamplerAddressMode addressModeU;
			vk::SamplerAddressMode addressModeV;
			vk::SamplerAddressMode addressModeW;
			float mipLodBias;
			vk::Bool32 anisotropyEnable;
			float maxAnisotropy;
			vk::Bool32 compareEnable;
			vk::CompareOp compareOp;
			float minLod;
			float maxLod;
			vk::BorderColor borderColor;
			vk::Bool32 unnormalizedCoordinates;

			SamplerInfo(vk::SamplerCreateFlags flags = vk::SamplerCreateFlags()
			    , vk::Filter magFilter = vk::Filter()
			    , vk::Filter minFilter = vk::Filter()
			    , vk::SamplerMipmapMode mipmapMode = vk::SamplerMipmapMode()
			    , vk::SamplerAddressMode addressModeU = vk::SamplerAddressMode()
			    , vk::SamplerAddressMode addressModeV = vk::SamplerAddressMode()
			    , vk::SamplerAddressMode addressModeW = vk::SamplerAddressMode()
			    , float mipLodBias = 0.0f
			    , vk::Bool32 anisotropyEnable = VK_FALSE
			    , float maxAnisotropy = 0.0f
			    , vk::Bool32 compareEnable = VK_FALSE
			    , vk::CompareOp compareOp = vk::CompareOp()
			    , float minLod = 0.0f
			    , float maxLod = 0.0f
			    , vk::BorderColor borderColor = vk::BorderColor()
			    , vk::Bool32 unnormalizedCoordinates = VK_FALSE
			);
		};
		
		struct SamplerCreateInfo {
			vk::SamplerCreateFlags flags;
			vk::Filter magFilter;
			vk::Filter minFilter;
			vk::SamplerMipmapMode mipmapMode;
			vk::SamplerAddressMode addressModeU;
			vk::SamplerAddressMode addressModeV;
			vk::SamplerAddressMode addressModeW;
			float mipLodBias;
			vk::Bool32 anisotropyEnable;
			float maxAnisotropy;
			float minLod;
			float maxLod;
			vk::BorderColor borderColor;

			SamplerCreateInfo(vk::SamplerCreateFlags flags = vk::SamplerCreateFlags()
			    , vk::Filter magFilter = vk::Filter()
			    , vk::Filter minFilter = vk::Filter()
			    , vk::SamplerMipmapMode mipmapMode = vk::SamplerMipmapMode()
			    , vk::SamplerAddressMode addressModeU = vk::SamplerAddressMode()
			    , vk::SamplerAddressMode addressModeV = vk::SamplerAddressMode()
			    , vk::SamplerAddressMode addressModeW = vk::SamplerAddressMode()
			    , float mipLodBias = 0.0f
			    , vk::Bool32 anisotropyEnable = VK_FALSE
			    , float maxAnisotropy = 0.0f
			    , float minLod = 0.0f
			    , float maxLod = 0.0f
				, vk::BorderColor borderColor = vk::BorderColor()
			);
		};

		struct Sampler {
		public:
		    Sampler(SamplerInfo info);
			SamplerInfo getInfo() const;
			const vk::Sampler *getSampler() const;

		private:
		    Sampler() = delete;
			SamplerInfo m_info;
			std::shared_ptr<vk::Sampler> m_pSampler;
			void _create();
		};

		Texture(vk::Format format
		    , Bool32 mipMap
			, Bool32 defaultImageView = VG_TRUE
			, Bool32 defaultSampler = VG_TRUE
			);
		~Texture();

		TextureType getType() const;
		Bool32 getIsMipmap() const;
		const Image *getImage() const;
		const ImageView *getImageView() const;
		const Sampler *getSampler() const;

		const ImageView *createImageView(std::string name, ImageViewCreateInfo createInfo);
		const ImageView *getImageView(std::string name) const;
		const Sampler *createSampler(std::string name, SamplerCreateInfo createInfo);
		const Sampler *getSampler(std::string name) const;
	protected:
		TextureType m_type;		
		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_depth;
		uint32_t m_arrayLength;
		vk::Format m_format;	
		Bool32 m_mipMap;
		uint32_t m_mipLevels;
		uint32_t m_arrayLayers;
		vk::ImageAspectFlags m_allAspectFlags;
		vk::ImageUsageFlags m_usageFlags;
		vk::ImageLayout m_layout;		
		Bool32 m_isCreateDefaultImageView;
		Bool32 m_isCreateDefaultSampler;

		TextureDataInfo m_dataLayout;
		std::vector<TextureDataInfo::Component> m_components;
		void *m_pMemory;
		uint32_t m_memorySize;
		uint32_t m_realSize;
		std::shared_ptr<Image> m_pImage;
		std::shared_ptr<ImageView> m_pImageView;
		std::shared_ptr<Sampler> m_pSampler;

		std::unordered_map<std::string, std::shared_ptr<ImageView>> m_mapPOtherImageViews;
		std::unordered_map<std::string, std::shared_ptr<Sampler>> m_mapPOtherSamplers;

		virtual void _init(Bool32 importContent);

		void _updateMipMapLevels();
		void _updateArrayLayer();
		void _createImage(Bool32 importContent);
		void _createImageView();
		void _createSampler();

		void _applyData(const TextureDataInfo &layoutInfo
			, const void *memory
			, uint32_t size
			, Bool32 cacheMemory = VG_FALSE
		    , Bool32 createMipmaps = VG_FALSE);

		vk::ImageViewType _getImageViewType() const;

		void _createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties,
			std::shared_ptr<vk::Buffer>& pBuffer, std::shared_ptr<vk::DeviceMemory>& pBufferMemory);
		void _tranImageLayout(std::shared_ptr<vk::CommandBuffer> &pCommandBuffer, vk::Image image,
			vk::ImageLayout oldLayout, vk::ImageLayout newLayout,
			uint32_t baseMipLevel, uint32_t levelCount,
			uint32_t baseArrayLayer, uint32_t layerCount);
		void _copyBufferToImage(std::shared_ptr<vk::CommandBuffer> &pCommandBuffer, vk::Buffer buffer, vk::Image image,
			uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevel,
			uint32_t baseArrayLayer, uint32_t layerCount);
	};
}

#endif // !VG_TEXTURE_H
