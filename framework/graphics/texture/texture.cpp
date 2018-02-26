#include "graphics/texture/texture.hpp"

namespace vg
{
	inline uint32_t caculateImageSizeWithMipmapLevel(uint32_t size, uint32_t mipmapLevel);

	Texture::Texture(TextureFormat format, Bool32 mipMap)
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
	{

	}

	Texture::~Texture()
	{

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
		//Need to receate sampler when chaning sampler address mode.
		_createSampler();
	}

	TextureType Texture::getType() const
	{
		return m_type;
	}

	TextureFormat Texture::getFormat() const
	{
		return m_format;
	}

	Bool32 Texture::getIsMipmap() const
	{
		return m_mipMap;
	}

	uint32_t Texture::getMipmapLevel() const
	{
		return m_mipMapLevels;
	}

	vk::Format Texture::getVKFormat() const
	{
		return m_vkFormat;
	}

	vk::ImageLayout Texture::getImageLayout() const
	{
		return m_vkImageLayout;
	}

	std::shared_ptr<vk::Image> Texture::getImage() const
	{
		return m_pImage;
	}

	std::shared_ptr<vk::DeviceMemory> Texture::getImageMemory() const
	{
		return m_pMemory;
	}

	std::shared_ptr<vk::ImageView> Texture::getImageView() const
	{
		return m_pImageView;
	}

	std::shared_ptr<vk::Sampler> Texture::getSampler() const
	{
		return m_pSampler;
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
		vk::Format vkFormat;
		for (const auto& item : arrFormatToVKFormat)
		{
			if (item.first == m_format)
			{
				vkFormat = item.second;
			}
		}

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

		const auto &pDevice = pApp->getDevice();
		m_pImage = fd::createImage(pDevice, createInfo);

		const auto &memRequirements = pDevice->getImageMemoryRequirements(*m_pImage);

		vk::MemoryAllocateInfo allocInfo = {
			memRequirements.size,
			vg::findMemoryType(pApp->getPhysicalDevice(), memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal)
		};

		m_pMemory = fd::allocateMemory(pDevice, allocInfo);

		pDevice->bindImageMemory(*m_pImage, *m_pMemory, vk::DeviceSize(0));

		m_currVkImageLayout = imageLayout;
	}

	void Texture::_createImageView()
	{

		vk::Format vkFormat;
		for (const auto& item : arrFormatToVKFormat)
		{
			if (item.first == m_format)
			{
				vkFormat = item.second;
			}
		}

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

		const auto &pDevice = pApp->getDevice();
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
			m_anisotropy == 0.0f ? VkBool32(VK_FALSE) : VkBool32(VK_TRUE),      //anisotropyEnable
			m_anisotropy,                            //maxAnisotropy
			VkBool32(VK_FALSE),                      //compareEnable
			vk::CompareOp::eNever,                   //compareOp
			0.0f,                                    //minLod
			0.0f,                                    //maxLod
			vk::BorderColor::eFloatTransparentBlack, //borderColor
			VkBool32(VK_FALSE)                       //unnormalizedCoordinates
		};

		const auto &pDevice = pApp->getDevice();
		m_pSampler = fd::createSampler(pDevice, createInfo);
	}

	std::vector<Color> Texture::_getPixels(uint32_t layer, uint32_t mipLevel) const
	{
		if (m_arrTempColors.size() <= mipLevel) return {};
		size_t oneLayerCount = m_width * m_height * m_depth;
		size_t start = oneLayerCount * layer;
		size_t size = m_arrTempColors[mipLevel].size();
		std::vector<Color> result(oneLayerCount);
		for (size_t i = start, j = 0; i < size && j < oneLayerCount; ++i, ++j)
		{
			result[j] = m_arrTempColors[mipLevel][i];
			result[j] /= 255.0f;
		}
		return result;
	}

	std::vector<Color32> Texture::_getPixels32(uint32_t layer, uint32_t mipLevel) const
	{
		if (m_arrTempColors.size() <= mipLevel) return {};
		size_t oneLayerCount = m_width * m_height * m_depth;
		size_t start = oneLayerCount * layer;
		size_t size = m_arrTempColors[mipLevel].size();
		std::vector<Color32> result(oneLayerCount);
		for (size_t i = start, j = 0; i < size && j < oneLayerCount; ++i, ++j)
		{
			result[j] = m_arrTempColors[mipLevel][i];
		}
		return result;
	}

	void Texture::_setPixels(const std::vector<Color> &colors, uint32_t layer, uint32_t mipLevel)
	{
		_resizeColorsData(mipLevel);
#ifdef DEBUG
		if (layer >= m_arrayLayer)
		{
			LOG(plog::warning) << "Invalid layer.";
			return;
		}
#endif // DEBUG
        uint32_t width = caculateImageSizeWithMipmapLevel(m_width, mipLevel);
		uint32_t height = caculateImageSizeWithMipmapLevel(m_height, mipLevel);
		uint32_t depth = caculateImageSizeWithMipmapLevel(m_depth, mipLevel);
		size_t oneLayerSize = width * height * depth;
		size_t start = oneLayerSize * layer;
		size_t size = oneLayerSize * m_arrayLayer;
		size_t colorsSize = colors.size();
		for (size_t i = 0, j = start; i < oneLayerSize && j < size; ++i, ++j)
		{
			auto tempColor = colors[i];
			tempColor *= 255.0f;
			tempColor.a = std::round(tempColor.a);
			tempColor.r = std::round(tempColor.r);
			tempColor.g = std::round(tempColor.g);
			tempColor.b = std::round(tempColor.b);
			if (i < colorsSize)m_arrTempColors[mipLevel][j] = tempColor;
			else break;
		}
	}

	void Texture::_setPixels(const void* colors, uint32_t size, uint32_t layer, uint32_t mipLevel)
	{
        uint32_t width = caculateImageSizeWithMipmapLevel(m_width, mipLevel);
		uint32_t height = caculateImageSizeWithMipmapLevel(m_height, mipLevel);
		uint32_t depth = caculateImageSizeWithMipmapLevel(m_depth, mipLevel);
		uint32_t validColorCount = width * height * depth;
		uint32_t validSize = validColorCount * static_cast<uint32_t>(sizeof(Color));
		validSize = std::min(validSize, size);			
		std::vector<Color> tempColors(validColorCount);
		memcpy(reinterpret_cast<void*>(tempColors.data()), colors, validSize);
		_setPixels(tempColors, layer, mipLevel);
	}

	void Texture::_setPixels32(const std::vector<Color32> &colors, uint32_t layer, uint32_t mipLevel)
	{
		_resizeColorsData(mipLevel);
#ifdef DEBUG
		if (layer >= m_arrayLayer)
		{
			LOG(plog::warning) << "Invalid layer.";
			return;
		}
#endif // DEBUG
        uint32_t width = caculateImageSizeWithMipmapLevel(m_width, mipLevel);
		uint32_t height = caculateImageSizeWithMipmapLevel(m_height, mipLevel);
		uint32_t depth = caculateImageSizeWithMipmapLevel(m_depth, mipLevel);
		size_t oneLayerSize = width * height * depth;
		size_t start = oneLayerSize * layer;
		size_t size = oneLayerSize * m_arrayLayer;
		size_t colorsSize = colors.size();
		for (size_t i = 0, j = start; i < oneLayerSize && j < size; ++i, ++j)
		{
			if (i < colorsSize)m_arrTempColors[mipLevel][j] = colors[i];
			else break;
		}
	}

	void Texture::_setPixels32(const void* colors, uint32_t size, uint32_t layer, uint32_t mipLevel)
	{
		_resizeColorsData(mipLevel);
#ifdef DEBUG
		if (layer >= m_arrayLayer)
		{
			LOG(plog::warning) << "Invalid layer.";
			return;
		}
#endif // DEBUG
		uint32_t width = caculateImageSizeWithMipmapLevel(m_width, mipLevel);
		uint32_t height = caculateImageSizeWithMipmapLevel(m_height, mipLevel);
		uint32_t depth = caculateImageSizeWithMipmapLevel(m_depth, mipLevel);
		uint32_t validColorCount = width * height * depth;
		uint32_t validSize = validColorCount * static_cast<uint32_t>(sizeof(Color32));
		validSize = std::min(validSize, size);
		size_t oneLayerSize = validColorCount;
		size_t start = oneLayerSize * layer;
		memcpy(reinterpret_cast<void*>(m_arrTempColors[mipLevel].data() + start), colors, validSize);
	}

	void Texture::_apply(Bool32 updateMipmaps, Bool32 makeUnreadable)
	{
		if (updateMipmaps)
		{
			_applyWithGenMipMap();
		}
		else
		{
			_applyDirect();
		}
		if (makeUnreadable)
		{
			//clear really colors data with reallocate.
			std::vector<std::vector<Color32>>().swap(m_arrTempColors);
		}
	}

	void Texture::_applyWithGenMipMap()
	{
		const auto &pDevice = pApp->getDevice();
		auto pCommandBuffer = beginSingleTimeCommands();
		//create first level image data using staging buffer.
		if (m_arrTempColors.size() == 0)
		{
			LOG(plog::warning) << "Texture data is empty at mipmap level 0.";
			return;
		}

		//create staging buffer.
		uint32_t imageSize = (m_width * m_height * m_depth * m_arrayLayer) * sizeof(Color32);
		std::shared_ptr<vk::Buffer> pStagingBuffer;
		std::shared_ptr<vk::DeviceMemory> pStagingBufferMemory;
		_createBuffer(imageSize, vk::BufferUsageFlagBits::eTransferSrc,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
			pStagingBuffer, pStagingBufferMemory);

		void *data = pDevice->mapMemory(*pStagingBufferMemory, 0, imageSize);
		memcpy(data, m_arrTempColors[0].data(), static_cast<size_t>(imageSize));
		pDevice->unmapMemory(*pStagingBufferMemory);

		//transfer image from initial current image layout to dst layout.
		//here use undefined layout not to use curr layout of image, it can clear image old data.
		_tranImageLayout(pCommandBuffer, *m_pImage, m_currVkImageLayout, vk::ImageLayout::eTransferDstOptimal,
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

		endSingleTimeCommands(pCommandBuffer);

		m_currVkImageLayout = m_vkImageLayout;
	}

	void Texture::_applyDirect()
	{
		if (m_arrTempColors.size() < m_mipMapLevels)
		{
			throw std::runtime_error("Pixels data is not enough to create mipmap texture.");
		}

		const auto &pDevice = pApp->getDevice();

		//create image data using staging buffer mipmap level by mipmap level.
		for (uint32_t i = 0; i < m_mipMapLevels; ++i)
		{
			auto pCommandBuffer = beginSingleTimeCommands();
			//create staging buffer.
			uint32_t imageSize = static_cast<uint32_t>(m_arrTempColors[i].size() * sizeof(Color32));
			std::shared_ptr<vk::Buffer> pStagingBuffer;
			std::shared_ptr<vk::DeviceMemory> pStagingBufferMemory;
			_createBuffer(imageSize, vk::BufferUsageFlagBits::eTransferSrc,
				vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
				pStagingBuffer, pStagingBufferMemory);

			void *data = pDevice->mapMemory(*pStagingBufferMemory, 0, imageSize);
			memcpy(data, m_arrTempColors[i].data(), static_cast<size_t>(imageSize));
			pDevice->unmapMemory(*pStagingBufferMemory);

			//transfer image from initial current image layout to dst layout.
			//here use undefined layout not to use curr layout of image, it can clear image old data.
			_tranImageLayout(pCommandBuffer, *m_pImage, m_currVkImageLayout, vk::ImageLayout::eTransferDstOptimal,
				i, 1, 0, m_arrayLayer);

			uint32_t width = caculateImageSizeWithMipmapLevel(m_width, i);
			uint32_t height = caculateImageSizeWithMipmapLevel(m_height, i);
			uint32_t depth = caculateImageSizeWithMipmapLevel(m_depth, i);
			_copyBufferToImage(pCommandBuffer, *pStagingBuffer, *m_pImage, width, height, depth, i, 0, m_arrayLayer);

			//transfer all layer to shader read layout.
			_tranImageLayout(pCommandBuffer, *m_pImage, vk::ImageLayout::eTransferDstOptimal, m_vkImageLayout,
				i, 1, 0, m_arrayLayer);

			endSingleTimeCommands(pCommandBuffer);
		}

		m_currVkImageLayout = m_vkImageLayout;
	}

	void Texture::_resizeColorsData(uint32_t mipLevel)
	{
		uint32_t width = caculateImageSizeWithMipmapLevel(m_width, mipLevel);
		uint32_t height = caculateImageSizeWithMipmapLevel(m_height, mipLevel);
		uint32_t depth = caculateImageSizeWithMipmapLevel(m_depth, mipLevel);
		size_t size = width * height * depth * m_arrayLayer;
		if (m_arrTempColors.size() < mipLevel + 1)m_arrTempColors.resize(mipLevel + 1);
		m_arrTempColors[mipLevel].resize(size);
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

		const auto &pDevice = pApp->getDevice();
		pBuffer = fd::createBuffer(pDevice, createInfo);

		vk::MemoryRequirements memReqs = pDevice->getBufferMemoryRequirements(*pBuffer);
		vk::MemoryAllocateInfo allocateInfo = {
			memReqs.size,
			vg::findMemoryType(pApp->getPhysicalDevice(), memReqs.memoryTypeBits, properties)
		};

		pBufferMemory = fd::allocateMemory(pDevice, allocateInfo);

		pDevice->bindBufferMemory(*pBuffer, *pBufferMemory, 0);
	}

	void  Texture::_tranImageLayout(std::shared_ptr<vk::CommandBuffer> pCommandBuffer, vk::Image image,
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

	void Texture::_copyBufferToImage(std::shared_ptr<vk::CommandBuffer> pCommandBuffer, vk::Buffer buffer, vk::Image image,
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

	inline uint32_t caculateImageSizeWithMipmapLevel(uint32_t size, uint32_t mipmapLevel)
	{
		return std::max(1u, size >> mipmapLevel);
	}
}