#include "graphics/texture/texture.hpp"

namespace vg
{
	inline uint32_t caculateImageSizeWithMipmapLevel(uint32_t size, uint32_t mipmapLevel);

	TextureDataInfo::Component::Component()
		: mipLevel(0u)
		, baseArrayLayer(0u)
		, layerCount(0u)
		, size(0u)
		, hasImageExtent(VG_FALSE)
		, width(0u)
		, height(0u)
		, depth(0u)
	{
	}

	TextureDataInfo::Component::Component(uint32_t mipLevel
		, uint32_t baseArrayLayer
		, uint32_t layerCount
		, uint32_t size
		, Bool32 hasImageExtent
		, uint32_t width
		, uint32_t height
		, uint32_t depth
	)
		: mipLevel(mipLevel)
		, baseArrayLayer(baseArrayLayer)
		, layerCount(layerCount)
		, size(size)
		, hasImageExtent(hasImageExtent)
		, width(width)
		, height(height)
		, depth(depth)
	{

	}

	TextureDataInfo::TextureDataInfo()
		: componentCount(0u)
		, pComponent(nullptr)
	{

	}

	Texture::Texture(vk::Format format, Bool32 mipMap)
		: Base(BaseType::TEXTURE)
		, m_format(format)
		, m_mipMap(mipMap)
		, m_width(1U)
		, m_height(1U)
		, m_depth(1U)
		, m_arrayLength(1U)
		, m_filterMode(FilterMode::LINEAR)
		, m_samplerAddressMode(SamplerAddressMode::CLAMP_TO_BORDER)
		, m_anisotropy(0.0f)
		, m_vkImageUsageFlags(vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled) //default image usage is for sampled texture.
		, m_vkImageLayout(vk::ImageLayout::eShaderReadOnlyOptimal) //default image layout is for sampled texture.
		, m_vkImageAspectFlags(vk::ImageAspectFlagBits::eColor) //default image aspect is for sampled texture.
		, m_dataLayout()
		, m_components()
		, m_pMemory(nullptr)
		, m_memorySize(0u)
	{

	}

	Texture::~Texture()
	{
		if (m_pMemory != nullptr)
		{
			free(m_pMemory);
		}
	}

	float Texture::getAnisotropy() const
	{
		return m_anisotropy;
	}

	void Texture::setAnisotropy(float value)
	{
		m_anisotropy = value;
		//Need to receate sampler when chaning anisotropy.
		_createSampler();
	}

	FilterMode Texture::getFilterMode() const
	{
		return m_filterMode;
	}

	void Texture::setFilterMode(FilterMode value)
	{
		m_filterMode = value;
		_updateVkFilter();
		//Need to receate sampler when chaning filterMode.
		_createSampler();
	}

	SamplerAddressMode Texture::getSamplerAddressMode() const
	{
		return m_samplerAddressMode;
	}

	void Texture::setSamplerAddressMode(SamplerAddressMode value)
	{
		m_samplerAddressMode = value;
		_updateVkSamplerAddressMode();
		//Need to receate sampler when chaning sampler address mode.
		_createSampler();
	}

	TextureType Texture::getType() const
	{
		return m_type;
	}

	vk::Format Texture::getFormat() const
	{
		return m_format;
	}

	Bool32 Texture::getIsMipmap() const
	{
		return m_mipMap;
	}

	uint32_t Texture::getMipmapLevels() const
	{
		return m_mipMapLevels;
	}

	uint32_t Texture::getArrayLayerCount() const
	{
		return m_arrayLayer;
	}

	vk::Format Texture::getVKFormat() const
	{
		return m_vkFormat;
	}

	vk::ImageLayout Texture::getImageLayout() const
	{
		return m_vkImageLayout;
	}

	vk::Image *Texture::getImage() const
	{
		return m_pImage.get();
	}

	vk::DeviceMemory *Texture::getImageMemory() const
	{
		return m_pImageMemory.get();
	}

	vk::ImageView *Texture::getImageView() const
	{
		return m_pImageView.get();
	}

	vk::Sampler *Texture::getSampler() const
	{
		return m_pSampler.get();
	}

	vk::ImageAspectFlags Texture::getImageAspectFlags() const
	{
		return m_vkImageAspectFlags;
	}

	void Texture::_init()
	{
		_updateMipMapLevels();
		_updateArrayLayer();
		_updateVkFormat();
		_updateVkFilter();
		_updateVkSamplerAddressMode();
		_createImage();
		_createImageView();
		_createSampler();
	}

	void Texture::_updateMipMapLevels()
	{
		if (m_mipMap)
		{
			// calculate num of mip maps
			// numLevels = 1 + floor(log2(max(w, h, d)))
			m_mipMapLevels = static_cast<uint32_t>(std::floor(std::log2(std::max({ m_width, m_height, m_depth }))) + 1);
		}
		else
		{
			m_mipMapLevels = 1;
		}
	}

	void Texture::_updateArrayLayer()
	{
		uint32_t arraylayer;
		switch (m_type)
		{
		case TextureType::TEX_1D:
		case TextureType::TEX_2D:
		case TextureType::TEX_2D_COLOR_ATTACHMENT:
		case TextureType::TEX_2D_DEPTH_STENCIL_ATTACHMENT:
		case TextureType::TEX_3D:
		case TextureType::COLOR_ATTACHMENT:
		case TextureType::DEPTH_STENCIL_ATTACHMENT:
		{
			arraylayer = 1U;
			break;
		}
		case TextureType::TEX_1D_ARRAY:
		case TextureType::TEX_2D_ARRAY:
		{
			arraylayer = m_arrayLength;
			break;
		}
		case TextureType::CUBE:
		{
			arraylayer = static_cast<uint32_t>(CubemapFace::RANGE_SIZE);
			break;
		}
		case TextureType::CUBE_ARRARY:
		{
			arraylayer = static_cast<uint32_t>(CubemapFace::RANGE_SIZE) * m_arrayLength;
			break;
		}
		default:
			throw std::logic_error("Lack of caculating compatible array layer and flag at creating iamge for texture when texture type is " + mapTextureTypeToName[m_type]);
			break;
		}

		m_arrayLayer = arraylayer;
	}

	void Texture::_updateVkFormat()
	{
		vk::Format vkFormat = m_format;

#ifdef DEBUG
		if (vkFormat == vk::Format::eUndefined)
		{
			throw std::invalid_argument("Invalid format argument at creating image for texture.");
		}
#endif // DEBUG

		m_vkFormat = vkFormat;
	}

	void Texture::_updateVkFilter()
	{
		for (const auto& item : arrFilerModeToVK)
		{
			if (std::get<0>(item) == m_filterMode)
			{
				m_vkFilter = std::get<1>(item);
				m_vkSamplerMipmapMode = std::get<2>(item);
				break;
			}
		}
	}

	void Texture::_updateVkSamplerAddressMode()
	{
		for (const auto& item : arrSamplerAddressModeToVK)
		{
			if (item.first == m_samplerAddressMode)
			{
				m_vkSamplerAddressMode = item.second;
			}
		}
	}

	void Texture::_createImage()
	{
#ifdef DEBUG
		Bool32 isFind;
#endif // DEBUG

		vk::ImageType vkImageType;
#ifdef DEBUG
		isFind = VG_FALSE;
#endif // DEBUG

		for (const auto& item : arrTextureTypeToVKImageType)
		{
			if (item.first == m_type)
			{
				vkImageType = item.second;
#ifdef DEBUG
				isFind = VG_TRUE;
#endif // DEBUG
			}
		}

#ifdef DEBUG
		if (isFind == VG_FALSE)
		{
			throw std::logic_error("Invalid texture type at creating image for texture.");
		}
#endif // DEBUG


#ifdef DEBUG
		//check whether other arguments is compatibility with texture type.
		switch (m_type)
		{
		case TextureType::TEX_1D:
		case TextureType::TEX_1D_ARRAY:
		{
			if (m_width == 0) throw std::invalid_argument("Invalid width argument at creating image for texture.");
			if (m_height != 1) throw std::invalid_argument("Invalid height argument at creating image for texture.");
			if (m_depth != 1) throw std::invalid_argument("Invalide depth argument at creating image for texture.");
			break;
		}
		case TextureType::TEX_2D:
		case TextureType::TEX_2D_ARRAY:
		case TextureType::COLOR_ATTACHMENT:
		case TextureType::DEPTH_STENCIL_ATTACHMENT:
		case TextureType::TEX_2D_COLOR_ATTACHMENT:
		case TextureType::TEX_2D_DEPTH_STENCIL_ATTACHMENT:
		{
			if (m_width == 0) throw std::invalid_argument("Invalid width argument at creating image for texture.");
			if (m_height == 0) throw std::invalid_argument("Invalid height argument at creating image for texture.");
			if (m_depth != 1) throw std::invalid_argument("Invalide depth argument at creating image for texture.");
			break;
		}
		case TextureType::CUBE:
		case TextureType::CUBE_ARRARY:
		{
			if (m_width == 0) throw std::invalid_argument("Invalid width argument at creating image for texture.");
			if (m_width != m_height) throw std::invalid_argument("Invalid width and height argument at creating image for texture, because they is not equal.");
			if (m_depth != 1) throw std::invalid_argument("Invalide depth argument at creating image for texture.");
			break;
		}
		case TextureType::TEX_3D:
		{
			if (m_width == 0) throw std::invalid_argument("Invalid width argument at creating image for texture.");
			if (m_height == 0) throw std::invalid_argument("Invalid height argument at creating image for texture.");
			if (m_depth == 0) throw std::invalid_argument("Invalid depth argument at creating image for texture.");
			break;
		}
		default:
			throw std::logic_error("Lack of checking argument at creating iamge for texture when texture type is " + mapTextureTypeToName[m_type]);
			break;
		}

#endif // DEBUG

		//caculate compatible array layer and flag.
		vk::ImageCreateFlags flags;
		switch (m_type)
		{
		case TextureType::TEX_1D:
		case TextureType::TEX_2D:
		case TextureType::TEX_2D_COLOR_ATTACHMENT:
		case TextureType::TEX_2D_DEPTH_STENCIL_ATTACHMENT:
		case TextureType::TEX_3D:
		{
			break;
		}
		case TextureType::TEX_1D_ARRAY:
		case TextureType::TEX_2D_ARRAY:
		{
			break;
		}
		case TextureType::COLOR_ATTACHMENT:
		case TextureType::DEPTH_STENCIL_ATTACHMENT:
		{
			break;
		}
		case TextureType::CUBE:
		{
			flags |= vk::ImageCreateFlagBits::eCubeCompatible;
			break;
		}
		case TextureType::CUBE_ARRARY:
		{
			flags |= vk::ImageCreateFlagBits::eCubeCompatible;
			break;
		}
		default:
			throw std::logic_error("Lack of caculating compatible array layer and flag at creating iamge for texture when texture type is " + mapTextureTypeToName[m_type]);
			break;
		}

		vk::ImageLayout imageLayout = vk::ImageLayout::eUndefined;

		vk::ImageCreateInfo createInfo = {
			flags,
			vkImageType,
			m_vkFormat,
			{
				m_width,
				m_height,
				m_depth
			},
			m_mipMapLevels,
			m_arrayLayer,
			vk::SampleCountFlagBits::e1,
			vk::ImageTiling::eOptimal,
			m_vkImageUsageFlags,
			vk::SharingMode::eExclusive,
			0U,
			nullptr,
			imageLayout
		};

		auto pDevice = pApp->getDevice();
		m_pImage = fd::createImage(pDevice, createInfo);

		const auto &memRequirements = pDevice->getImageMemoryRequirements(*m_pImage);

		vk::MemoryAllocateInfo allocInfo = {
			memRequirements.size,
			vg::findMemoryType(pApp->getPhysicalDevice(), memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal)
		};

		m_pImageMemory = fd::allocateMemory(pDevice, allocInfo);

		pDevice->bindImageMemory(*m_pImage, *m_pImageMemory, vk::DeviceSize(0));

		m_usingVkImageLayout = imageLayout;
	}

	void Texture::_createImageView()
	{

		vk::Format vkFormat = m_format;

#ifdef DEBUG
		if (vkFormat == vk::Format::eUndefined)
		{
			throw std::invalid_argument("Invalid format argument at creating image for texture.");
		}
#endif // DEBUG

#ifdef DEBUG
		Bool32 isFind;
#endif // DEBUG
		vk::ImageViewType vkImageViewType;
#ifdef DEBUG
		isFind = VG_FALSE;
#endif // DEBUG

		for (const auto& item : arrTextureTypeToVKImageViewType)
		{
			if (item.first == m_type)
			{
				vkImageViewType = item.second;
#ifdef DEBUG
				isFind = VG_TRUE;
#endif // DEBUG
			}
		}

#ifdef DEBUG
		if (isFind == VG_FALSE)
		{
			throw std::invalid_argument("Invalid type argument at creating image view for texture.");
		}
#endif // DEBUG

		vk::ImageViewCreateInfo createInfo = {
			vk::ImageViewCreateFlags(),
			*m_pImage,
			vkImageViewType,
			vkFormat,
			{
				vk::ComponentSwizzle::eIdentity,
				vk::ComponentSwizzle::eIdentity,
				vk::ComponentSwizzle::eIdentity,
				vk::ComponentSwizzle::eIdentity
			},
			{
				m_vkImageAspectFlags,
				uint32_t(0),
				m_mipMapLevels,
				uint32_t(0),
				m_arrayLayer
			}
		};

		auto pDevice = pApp->getDevice();
		m_pImageView = fd::createImageView(pDevice, createInfo);
	}

	void Texture::_createSampler()
	{

		vk::SamplerCreateInfo createInfo = {
			vk::SamplerCreateFlags(),                //flags
			m_vkFilter,                              //magFilter
			m_vkFilter,                              //minFilter
			m_vkSamplerMipmapMode,                   //mipmapMode
			m_vkSamplerAddressMode,                  //addressModeU
			m_vkSamplerAddressMode,                  //addressModeV
			m_vkSamplerAddressMode,                  //addressModeW
			0.0f,                                    //mipLodBias
			m_anisotropy <= 0.0f ? VkBool32(VK_FALSE) : VkBool32(VK_TRUE),      //anisotropyEnable
			m_anisotropy <= 0.0f ? 1.0f : m_anisotropy, //maxAnisotropy
			VkBool32(VK_FALSE),                      //compareEnable
			vk::CompareOp::eNever,                   //compareOp
			0.0f,                                    //minLod
			m_mipMap ? m_mipMapLevels : 0.0f,        //maxLod
			vk::BorderColor::eFloatTransparentBlack, //borderColor
			VkBool32(VK_FALSE)                       //unnormalizedCoordinates
		};

		auto pDevice = pApp->getDevice();
		m_pSampler = fd::createSampler(pDevice, createInfo);
	}

	void Texture::_applyData(const TextureDataInfo &layoutInfo
		, const void *memory
		, uint32_t size
		, Bool32 cacheMemory
		, Bool32 createMipmaps)
	{
		if (cacheMemory)
		{
			m_components.resize(layoutInfo.componentCount);
			memcpy(m_components.data(), layoutInfo.pComponent,
				sizeof(TextureDataInfo::Component) * layoutInfo.componentCount);
			m_dataLayout = layoutInfo;
			m_dataLayout.pComponent = m_components.data();
		}

		if (m_pMemory != nullptr && (m_memorySize < size || ! cacheMemory)) {
			free(m_pMemory);
			m_pMemory = nullptr;
			m_memorySize = 0;
		}

		if (size)
		{
			if (cacheMemory) 
			{
				if (m_pMemory == nullptr)
				{
					m_pMemory = malloc(size);
					m_memorySize = size;
				}
				memcpy(m_pMemory, memory, size);
				m_realSize = size;
			}
			auto pDevice = pApp->getDevice();

			//create staging buffer.
			std::shared_ptr<vk::Buffer> pStagingBuffer;
			std::shared_ptr<vk::DeviceMemory> pStagingBufferMemory;
			_createBuffer(size, vk::BufferUsageFlagBits::eTransferSrc,
				vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
				pStagingBuffer, pStagingBufferMemory);

			void *data = pDevice->mapMemory(*pStagingBufferMemory, 0, size);
			memcpy(data, memory, static_cast<size_t>(size));
			pDevice->unmapMemory(*pStagingBufferMemory);
			auto pCommandBuffer = beginSingleTimeCommands();
			if (createMipmaps)
			{
				//transfer image from initial current image layout to dst layout.
				//here use undefined layout not to use curr layout of image, it can clear image old data.
				_tranImageLayout(pCommandBuffer, *m_pImage, m_usingVkImageLayout, vk::ImageLayout::eTransferDstOptimal,
					0, 1, 0, m_arrayLayer);

				//copy the first mip of the chain.
				_copyBufferToImage(pCommandBuffer, *pStagingBuffer, *m_pImage, m_width, m_height, m_depth, 0, 0, m_arrayLayer);

#ifdef DEBUG
				//check format.
				const auto &pPhysicalDevice = pApp->getPhysicalDevice();
				const auto &formatProperties = pPhysicalDevice->getFormatProperties(m_vkFormat);
				if ((formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eBlitSrc) == vk::FormatFeatureFlags())
				{
					throw std::runtime_error("The texture format don't support for blit source, mip-chain generation requires it.");
				}
				if ((formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eBlitDst) == vk::FormatFeatureFlags())
				{
					throw std::runtime_error("The texture format don't support for blit destination, mip-chain generation requires it.");
				}
#endif // DEBUG

				//transition first mip level to transfer source for read during blit.
				_tranImageLayout(pCommandBuffer, *m_pImage, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eTransferSrcOptimal, 0, 1, 0, m_arrayLayer);

				for (uint32_t i = 1; i < m_mipMapLevels; ++i)
				{
					vk::ImageBlit blit;
					blit.srcSubresource.aspectMask = m_vkImageAspectFlags;
					blit.srcSubresource.baseArrayLayer = 0;
					blit.srcSubresource.layerCount = m_arrayLayer;
					blit.srcSubresource.mipLevel = i - 1;
					blit.dstSubresource.aspectMask = m_vkImageAspectFlags;
					blit.dstSubresource.baseArrayLayer = 0;
					blit.dstSubresource.layerCount = m_arrayLayer;
					blit.dstSubresource.mipLevel = i;

					// each mipmap is the size divided by two
					blit.srcOffsets[1] = vk::Offset3D(caculateImageSizeWithMipmapLevel(m_width, i - 1),
						caculateImageSizeWithMipmapLevel(m_height, i - 1),
						caculateImageSizeWithMipmapLevel(m_depth, i - 1));

					blit.dstOffsets[1] = vk::Offset3D(caculateImageSizeWithMipmapLevel(m_width, i),
						caculateImageSizeWithMipmapLevel(m_height, i),
						caculateImageSizeWithMipmapLevel(m_depth, i));

					// transferDst go to transferSrc because this mipmap will be the source for the next iteration (the next level)
					_tranImageLayout(pCommandBuffer, *m_pImage, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal,
						i, 1, 0, m_arrayLayer);

					pCommandBuffer->blitImage(*m_pImage, vk::ImageLayout::eTransferSrcOptimal,
						*m_pImage, vk::ImageLayout::eTransferDstOptimal, blit,
						vk::Filter::eLinear);

					_tranImageLayout(pCommandBuffer, *m_pImage, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eTransferSrcOptimal,
						i, 1, 0, m_arrayLayer);
				}

				//transfer all level and all layer to shader read layout.
				_tranImageLayout(pCommandBuffer, *m_pImage, vk::ImageLayout::eTransferSrcOptimal, m_vkImageLayout,
					0, m_mipMapLevels, 0, m_arrayLayer);
			}
			else
			{
				const auto pComponent = layoutInfo.pComponent;
				const auto componentCount = layoutInfo.componentCount;
				std::vector<vk::BufferImageCopy> bufferCopyRegions(componentCount);
				uint32_t offset = 0u;
				for (uint32_t i = 0u; i < componentCount; ++i)
				{
					uint32_t width;
					uint32_t height;
					uint32_t depth;
					const auto component = *(layoutInfo.pComponent + i);
					if (component.hasImageExtent)
					{
						width = component.width;
						height = component.height;
						depth = component.depth;
					}
					else
					{
						width = caculateImageSizeWithMipmapLevel(m_width, component.mipLevel);
						height = caculateImageSizeWithMipmapLevel(m_height, component.mipLevel);
						depth = caculateImageSizeWithMipmapLevel(m_depth, component.mipLevel);
					}
					vk::ImageSubresourceLayers subresourceLayers = {
						m_vkImageAspectFlags,              //aspectMask
						component.mipLevel,            //mipLevel
						component.baseArrayLayer,      //baseArrayLayer
						component.layerCount           //layerCount
					};
					vk::BufferImageCopy copyInfo = { 
						offset,                                 //bufferOffset
						0,                                      //bufferRowLength
						0,                                      //bufferImageHeight
						subresourceLayers,                      //imageSubresource
						vk::Offset3D(0, 0, 0),                  //imageOffset
						vk::Extent3D(width, height, depth)      //imageExtent
					};

					bufferCopyRegions[i] = copyInfo;
					offset += component.size;
				}
				//transfer image from initial current image layout to dst layout.
				//here use undefined layout not to use curr layout of image, it can clear image old data.
				_tranImageLayout(pCommandBuffer, *m_pImage, m_usingVkImageLayout, vk::ImageLayout::eTransferDstOptimal,
					0, m_mipMapLevels, 0, m_arrayLayer);
				
				pCommandBuffer->copyBufferToImage(*pStagingBuffer, *m_pImage, vk::ImageLayout::eTransferDstOptimal, bufferCopyRegions);

				//transfer to shader read layout.
				_tranImageLayout(pCommandBuffer, *m_pImage, vk::ImageLayout::eTransferDstOptimal, m_vkImageLayout,
					0, m_mipMapLevels, 0, m_arrayLayer);

			}

			endSingleTimeCommands(pCommandBuffer);
			m_usingVkImageLayout = m_vkImageLayout;
		}
	}

	void Texture::_createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties,
		std::shared_ptr<vk::Buffer>& pBuffer, std::shared_ptr<vk::DeviceMemory>& pBufferMemory)
	{
		vk::BufferCreateInfo createInfo = {
			vk::BufferCreateFlags(),
			size,
			usage,
			vk::SharingMode::eExclusive
		};

		auto pDevice = pApp->getDevice();
		pBuffer = fd::createBuffer(pDevice, createInfo);

		vk::MemoryRequirements memReqs = pDevice->getBufferMemoryRequirements(*pBuffer);
		vk::MemoryAllocateInfo allocateInfo = {
			memReqs.size,
			vg::findMemoryType(pApp->getPhysicalDevice(), memReqs.memoryTypeBits, properties)
		};

		pBufferMemory = fd::allocateMemory(pDevice, allocateInfo);

		pDevice->bindBufferMemory(*pBuffer, *pBufferMemory, 0);
	}

	void  Texture::_tranImageLayout(std::shared_ptr<vk::CommandBuffer> &pCommandBuffer, vk::Image image,
		vk::ImageLayout oldLayout, vk::ImageLayout newLayout,
		uint32_t baseMipLevel, uint32_t levelCount,
		uint32_t baseArrayLayer, uint32_t layerCount)
	{
		vk::ImageMemoryBarrier barrier = {};
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;

		barrier.subresourceRange.aspectMask = m_vkImageAspectFlags;
		barrier.subresourceRange.baseMipLevel = baseMipLevel;
		barrier.subresourceRange.levelCount = levelCount;
		barrier.subresourceRange.baseArrayLayer = baseArrayLayer;
		barrier.subresourceRange.layerCount = layerCount;

		std::vector<std::tuple<vk::ImageLayout, vk::AccessFlags, vk::PipelineStageFlags>> arrLayoutToAccess = {
			std::tuple<vk::ImageLayout, vk::AccessFlags, vk::PipelineStageFlags>(vk::ImageLayout::eUndefined
				, vk::AccessFlags()
				, vk::PipelineStageFlagBits::eTopOfPipe
				)
			, std::tuple<vk::ImageLayout, vk::AccessFlags, vk::PipelineStageFlags>(vk::ImageLayout::eTransferDstOptimal
				, vk::AccessFlagBits::eTransferWrite
				, vk::PipelineStageFlagBits::eTransfer
				)
			, std::tuple<vk::ImageLayout, vk::AccessFlags, vk::PipelineStageFlags>(vk::ImageLayout::eTransferSrcOptimal
				, vk::AccessFlagBits::eTransferRead
				, vk::PipelineStageFlagBits::eTransfer
				)
			, std::tuple<vk::ImageLayout, vk::AccessFlags, vk::PipelineStageFlags>(vk::ImageLayout::eShaderReadOnlyOptimal
				, vk::AccessFlagBits::eShaderRead
				, vk::PipelineStageFlagBits::eFragmentShader
				)
			, std::tuple<vk::ImageLayout, vk::AccessFlags, vk::PipelineStageFlags>(vk::ImageLayout::eDepthStencilAttachmentOptimal
				, vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite
				, vk::PipelineStageFlagBits::eEarlyFragmentTests
				)
		};
		vk::PipelineStageFlags srcStageMask;
		vk::PipelineStageFlags dstStageMask;
		Bool32 isFindSrc = VG_FALSE;
		Bool32 isFindDst = VG_FALSE;
		for (const auto& item : arrLayoutToAccess)
		{
			if (oldLayout == std::get<0>(item))
			{
				barrier.srcAccessMask = std::get<1>(item);
				srcStageMask = std::get<2>(item);
				isFindSrc = VG_TRUE;
			}
			if (newLayout == std::get<0>(item))
			{
				barrier.dstAccessMask = std::get<1>(item);
				dstStageMask = std::get<2>(item);
				isFindDst = VG_TRUE;
			}
			if (isFindSrc && isFindDst)
			{
				break;
			}
		}

		pCommandBuffer->pipelineBarrier(srcStageMask,
			dstStageMask,
			vk::DependencyFlags(),
			nullptr, nullptr,
			barrier);
	}

	inline uint32_t caculateImageSizeWithMipmapLevel(uint32_t size, uint32_t mipmapLevel)
	{
		return std::max(1u, size >> mipmapLevel);
	}

	void Texture::_copyBufferToImage(std::shared_ptr<vk::CommandBuffer> &pCommandBuffer, vk::Buffer buffer, vk::Image image,
		uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevel,
		uint32_t baseArrayLayer, uint32_t layerCount)
	{
		vk::BufferImageCopy copyInfo = { 0, 0, 0, vk::ImageSubresourceLayers(
			m_vkImageAspectFlags, mipLevel, baseArrayLayer, layerCount),
			vk::Offset3D(0, 0, 0),
			vk::Extent3D(width, height, depth)
		};

		pCommandBuffer->copyBufferToImage(buffer, image, vk::ImageLayout::eTransferDstOptimal, copyInfo);
	}
}