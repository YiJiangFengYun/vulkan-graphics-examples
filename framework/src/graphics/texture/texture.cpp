#include "graphics/texture/texture.hpp"

namespace kgs
{
	Texture::Texture(Device device, TextureFormat format, Bool32 mipMap)
		:m_device(device),
		m_format(format),
		m_mipMap(mipMap),
		m_width(1U),
		m_height(1U),
		m_depth(1U),
		m_arrayLength(1U)
	{
		
	}

	Texture::~Texture()
	{

	}

	void Texture::_caculateMipMapLevels()
	{
		if (m_mipMap)
		{
			// calculate num of mip maps
			// numLevels = 1 + floor(log2(max(w, h, d)))
			m_mipMapLevels = std::floor(std::log2(std::max(m_width, m_height, m_depth))) + 1;
		}
		else
		{
			m_mipMapLevels = 1;
		}
	}

	void Texture::_caculateArrayLayer()
	{
		uint32_t arraylayer;
		switch (m_type)
		{
		case TextureType::TEX_1D:
		case TextureType::TEX_2D:
		case TextureType::TEX_3D:
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
			arraylayer = 6U;
			break;
		}
		case TextureType::CUBE_ARRARY:
		{
			arraylayer = 6U * m_arrayLength;
			break;
		}
		default:
			throw std::logic_error("Lack of caculating compatible array layer and flag at creating iamge for texture when texture type is " + mapTextureTypeToName[m_type]);
			break;
		}

		m_arrayLayer = arraylayer;
	}

	void Texture::_caculateVkFormat()
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

	void Texture::_createImage()
	{
#ifdef DEBUG
		Bool32 isFind;
#endif // DEBUG

		vk::ImageType vkImageType;
#ifdef DEBUG
		isFind = KGS_FALSE;
#endif // DEBUG

		for (const auto& item : arrTextureTypeToVKImageType)
		{
			if (item.first == m_type)
			{
				vkImageType = item.second;
#ifdef DEBUG
				isFind = KGS_TRUE;
#endif // DEBUG
			}
		}

#ifdef DEBUG
		if (isFind == KGS_FALSE)
		{
			throw std::invalid_argument("Invalid type argument at creating image for texture.");
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
			vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
			vk::SharingMode::eExclusive,
			0U,
			nullptr,
			imageLayout
		};

		vk::Device nativeDevice = m_device.getNativeDevice();
		m_image = nativeDevice.createImage(createInfo, nullptr);

		auto memRequirements = nativeDevice.getImageMemoryRequirements(m_image);

		vk::MemoryAllocateInfo allocInfo = {
			memRequirements.size,
			_findMemoryType(memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal)
		};

		m_memory = nativeDevice.allocateMemory(allocInfo, nullptr);

		nativeDevice.bindImageMemory(m_image, m_memory, vk::DeviceSize(0));

		m_vkImageLayout = imageLayout;
	}

	void Texture::_destroyImage()
	{
		vk::Device nativeDevice = m_device.getNativeDevice();
		nativeDevice.destroyImage(m_image);
		nativeDevice.freeMemory(m_memory);
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
		isFind = KGS_FALSE;
#endif // DEBUG

		for (const auto& item : arrTextureTypeToVKImageViewType)
		{
			if (item.first == m_type)
			{
				vkImageViewType = item.second;
#ifdef DEBUG
				isFind = KGS_TRUE;
#endif // DEBUG
			}
		}

#ifdef DEBUG
		if (isFind == KGS_FALSE)
		{
			throw std::invalid_argument("Invalid type argument at creating image view for texture.");
		}
#endif // DEBUG

		vk::ImageViewCreateInfo createInfo = {
			vk::ImageViewCreateFlags(),
			m_image,
			vkImageViewType,
			vkFormat,
			{
				vk::ComponentSwizzle::eIdentity,
				vk::ComponentSwizzle::eIdentity,
				vk::ComponentSwizzle::eIdentity,
				vk::ComponentSwizzle::eIdentity
			},
			{
				vk::ImageAspectFlagBits::eColor,
				uint32_t(0),
				m_mipMapLevels,
				uint32_t(0),
				m_arrayLayer
			}
		};

		vk::Device nativeDevice = m_device.getNativeDevice();
		m_imageView = nativeDevice.createImageView(createInfo);
	}

	void Texture::_destroyImageView()
	{
		vk::Device nativeDevice = m_device.getNativeDevice();
		nativeDevice.destroyImageView(m_imageView);
	}

	void Texture::_createSampler(FilterMode filterMode, SamplerAddressMode samplerAddressMode, float anisotropy)
	{
		vk::Filter vkFiler;
		vk::SamplerMipmapMode vkSamplerMipmapMode;
		for (const auto& item : arrFilerModeToVK)
		{
			if (std::get<0>(item) == filterMode)
			{
				vkFiler = std::get<1>(item);
				vkSamplerMipmapMode = std::get<2>(item);
			}
		}

		vk::SamplerAddressMode vkSamplerAddressMode;
		for (const auto& item : arrSamplerAddressModeToVK)
		{
			if (item.first == samplerAddressMode)
			{
				vkSamplerAddressMode = item.second;
			}
		}


		vk::SamplerCreateInfo createInfo = {
			vk::SamplerCreateFlags(),
			vkFiler,
			vkFiler,
			vkSamplerMipmapMode,
			vkSamplerAddressMode,
			vkSamplerAddressMode,
			vkSamplerAddressMode,
			0.0f,
			anisotropy == 0.0f ? VkBool32(VK_FALSE) : VkBool32(VK_TRUE),
			anisotropy,
			VkBool32(VK_FALSE),
			vk::CompareOp::eNever,
			0.0f,
			0.0f,
			vk::BorderColor::eFloatTransparentBlack,
			VkBool32(VK_FALSE)
		};

		vk::Device nativeDevice = m_device.getNativeDevice();
		m_sampler = nativeDevice.createSampler(createInfo);
	}

	void Texture::_destroySampler()
	{
		vk::Device nativeDevice = m_device.getNativeDevice();
		nativeDevice.destroySampler(m_sampler);
	}

	std::vector<Color> Texture::_getPixels(uint32_t layer, uint32_t mipLevel)
	{
		if (arrTempColors.size() <= mipLevel) return {};
		size_t oneLayerCount = m_width * m_height * m_depth;
		size_t start = oneLayerCount * layer;
		size_t size = arrTempColors[mipLevel].size();
		std::vector<Color> result(oneLayerCount);
		for (size_t i = start, j = 0; i < size && j < oneLayerCount; ++i, ++j)
		{
			result[j] = arrTempColors[mipLevel][i];
		}
		return result;
	}

	std::vector<Color32> Texture::_getPixels32(uint32_t layer, uint32_t mipLevel)
	{
		if (arrTempColors.size() <= mipLevel) return {};
		size_t oneLayerCount = m_width * m_height * m_depth;
		size_t start = oneLayerCount * layer;
		size_t size = arrTempColors[mipLevel].size();
		std::vector<Color32> result(oneLayerCount);
		for (size_t i = start, j = 0; i < size && j < oneLayerCount; ++i, ++j)
		{
			result[j] = arrTempColors[mipLevel][i];
		}
		return result;
	}

	void Texture::_setPixels(std::vector<Color> colors, uint32_t layer, uint32_t mipLevel)
	{
		_resizeColorsData(mipLevel);
#ifdef DEBUG
		if(layer >= m_arrayLayer)
		{
			LOG(plog::warning) << "Invalid layer.";
			return;
		}
#endif // DEBUG
		size_t oneLayerSize = m_width * m_height * m_depth;
		size_t start = oneLayerSize * layer;
		size_t size = oneLayerSize * m_arrayLayer;
		size_t colorsSize = colors.size();
		for (size_t i = 0, j = start; i < oneLayerSize && j < size; ++i, ++j)
		{
			if(i < colorsSize)arrTempColors[mipLevel][j] = colors[i];
		}
	}

	void Texture::_setPixels32(std::vector<Color32> colors, uint32_t layer, uint32_t mipLevel)
	{
		_resizeColorsData(mipLevel);
#ifdef DEBUG
		if (layer >= m_arrayLayer)
		{
			LOG(plog::warning) << "Invalid layer.";
			return;
		}
#endif // DEBUG
		size_t oneLayerSize = m_width * m_height * m_depth;
		size_t start = oneLayerSize * layer;
		size_t size = oneLayerSize * m_arrayLayer;
		size_t colorsSize = colors.size();
		for (size_t i = 0, j = start; i < oneLayerSize && j < size; ++i, ++j)
		{
			if (i < colorsSize)arrTempColors[mipLevel][j] = colors[i];
		}
	}

	void Texture::_apply(Bool32 updateMipmaps, Bool32 makeUnreadable)
	{
		vk::Device device = m_device.getNativeDevice();
		vk::CommandBuffer commandBuffer = _beginSingleTimeCommands();
		//create first level image data using staging buffer.
			if (arrTempColors.size() == 0)
			{
				LOG(plog::warning) << "Texture data is empty at mipmap level 0.";
				return;
			}
			if (arrTempColors[0].size() < m_arrayLayer)
			{
				LOG(plog::warning) << "Texture data is not enough for array layer: " << m_arrayLayer << " at mipmap level 0.";
				return;
			}

			//create staging buffer.
			uint32_t imageSize = (m_width * m_height * m_depth) * sizeof(Color32);
			vk::Buffer stagingBuffer;
			vk::DeviceMemory stagingBufferMemory;
			_createBuffer(imageSize, vk::BufferUsageFlagBits::eTransferSrc,
				vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
				stagingBuffer, stagingBufferMemory);

			void *data = device.mapMemory(stagingBufferMemory, 0, imageSize);
			memcpy(data, arrTempColors[0].data(), static_cast<size_t>(imageSize));
			vkUnmapMemory(device, stagingBufferMemory);

			//transfer image from initial current image layout to dst layout.
			//here use undefined layout not to use curr layout of image, it can clear image old data.
			_tranImageLayout(commandBuffer, m_image, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal,
			 0, 1, 0, m_arrayLayer);

			//copy the first mip of the chain.
			_copyBufferToImage(commandBuffer, stagingBuffer, m_image, m_width, m_height, m_depth, 0, 0, m_arrayLayer);
		

		//todo handle manual mip map
		if (updateMipmaps == false)
		{
			throw std::invalid_argument("UpdateMipmaps argument must be true now.");
		}
		else
		{
#ifdef DEBUG
			//check format.
			vk::PhysicalDevice physicalDevice = m_device.getPhysicalDevice();
			auto formatProperties = physicalDevice.getFormatProperties(m_vkFormat);
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
			_tranImageLayout(commandBuffer, m_image, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eTransferSrcOptimal, 0, 1, 0, m_arrayLayer);

			for (uint32_t i = 1; i < m_mipMapLevels; ++i) {
				vk::ImageBlit blit;
				blit.srcSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
				blit.srcSubresource.baseArrayLayer = 0;
				blit.srcSubresource.layerCount = m_arrayLayer;
				blit.srcSubresource.mipLevel = i - 1;
				blit.dstSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
				blit.dstSubresource.baseArrayLayer = 0;
				blit.dstSubresource.layerCount = m_arrayLayer;
				blit.dstSubresource.mipLevel = i;

				// each mipmap is the size divided by two
				blit.srcOffsets[1] = vk::Offset3D(std::max(1u, m_width >> (i - 1)),
					std::max(1u, m_height >> (i - 1)),
					std::max(1u, m_depth >> (i - 1)));

				blit.dstOffsets[1] = vk::Offset3D(std::max(1u, m_width >> i),
					std::max(1u, m_height >> i),
					std::max(1u, m_depth >> i));

				// transferDst go to transferSrc because this mipmap will be the source for the next iteration (the next level)
				_tranImageLayout(commandBuffer, m_image, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal,
					i, 1, 0, m_arrayLayer);

				commandBuffer.blitImage(m_image, vk::ImageLayout::eTransferSrcOptimal,
					m_image, vk::ImageLayout::eTransferDstOptimal, blit,
					vk::Filter::eLinear);

				_tranImageLayout(commandBuffer, m_image, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eTransferSrcOptimal,
					i, 1, 0, m_arrayLayer);
			}

			//transfer all level and all layer to shader read layout.
			_tranImageLayout(commandBuffer, m_image, vk::ImageLayout::eTransferSrcOptimal, vk::ImageLayout::eShaderReadOnlyOptimal,
				0, m_mipMapLevels, 0, m_arrayLayer);

			_endSingleTimeCommands(commandBuffer);

			//clear staging buffer resource.
			device.freeMemory(stagingBufferMemory);
			device.destroyBuffer(stagingBuffer);
		}
	}

	void Texture::_resizeColorsData(uint32_t mipLevel)
	{
		size_t size = m_width * m_height * m_depth * m_arrayLayer;
		if (arrTempColors.size() < mipLevel + 1)arrTempColors.resize(mipLevel + 1);
		arrTempColors[mipLevel].resize(size);
	}

	void Texture::_createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties,
		vk::Buffer& buffer, vk::DeviceMemory& bufferMemory)
	{
		vk::BufferCreateInfo createInfo = {
			vk::BufferCreateFlags(),
			size,
			usage,
			vk::SharingMode::eExclusive
		};

		vk::Device device = m_device.getNativeDevice();
		buffer = device.createBuffer(createInfo);

		vk::MemoryRequirements memReqs = device.getBufferMemoryRequirements(buffer);
		vk::MemoryAllocateInfo allocateInfo = {
			memReqs.size,
			_findMemoryType(memReqs.memoryTypeBits, properties)
		};
		bufferMemory = device.allocateMemory(allocateInfo);

		vkBindBufferMemory(device, buffer, bufferMemory, 0);
	}

	void  Texture::_tranImageLayout(vk::CommandBuffer commandBuffer, vk::Image image,
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

		barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		barrier.subresourceRange.baseMipLevel = baseMipLevel;
		barrier.subresourceRange.levelCount = levelCount;
		barrier.subresourceRange.baseArrayLayer = baseArrayLayer;
		barrier.subresourceRange.layerCount = layerCount;

		std::vector<std::pair<vk::ImageLayout, vk::AccessFlags>> arrLayoutToAccess = {
			std::pair<vk::ImageLayout, vk::AccessFlags>(vk::ImageLayout::eUndefined, vk::AccessFlags()),
			std::pair<vk::ImageLayout, vk::AccessFlags>(vk::ImageLayout::eTransferDstOptimal, vk::AccessFlagBits::eTransferWrite),
			std::pair<vk::ImageLayout, vk::AccessFlags>(vk::ImageLayout::eTransferSrcOptimal, vk::AccessFlagBits::eTransferWrite),
			std::pair<vk::ImageLayout, vk::AccessFlags>(vk::ImageLayout::eShaderReadOnlyOptimal, vk::AccessFlagBits::eShaderRead),
		};

		for (const auto& item : arrLayoutToAccess)
		{
			if (oldLayout == item.first)
			{
				barrier.srcAccessMask = item.second;
				break;
			}
		}

		for (const auto& item : arrLayoutToAccess)
		{
			if (newLayout == item.first)
			{
				barrier.dstAccessMask = item.second;
				break;
			}
		}

		commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe,
			vk::PipelineStageFlagBits::eTopOfPipe,
			vk::DependencyFlags(),
			nullptr, nullptr,
			barrier);
	}

	void Texture::_copyBufferToImage(vk::CommandBuffer commandBuffer, vk::Buffer buffer, vk::Image image,
		uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevel,
		uint32_t baseArrayLayer, uint32_t layerCount)
	{
		vk::BufferImageCopy copyInfo = { 0, 0, 0, vk::ImageSubresourceLayers(
			vk::ImageAspectFlagBits::eColor, mipLevel, baseArrayLayer, layerCount),
			vk::Offset3D(0, 0, 0),
			vk::Extent3D(width, height, depth)
		};

		commandBuffer.copyBufferToImage(buffer, image, vk::ImageLayout::eTransferDstOptimal, copyInfo);
	}

	uint32_t Texture::_findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties)
	{
		vk::PhysicalDevice physicalDevice = m_device.getPhysicalDevice();
		vk::PhysicalDeviceMemoryProperties memProperties = physicalDevice.getMemoryProperties();
		for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
		{
			if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}

		throw std::runtime_error("Failed to find suitable memory type!");

	}

	vk::CommandBuffer Texture::_beginSingleTimeCommands() {
		vk::Device device = m_device.getNativeDevice();
		vk::CommandPool commandPool = m_device.getCommandPool();
		vk::CommandBufferAllocateInfo allocateInfo = {
			commandPool,
			vk::CommandBufferLevel::ePrimary,
			uint32_t(1)
		};
		vk::CommandBuffer commandBuffer = device.allocateCommandBuffers(allocateInfo)[0];

		vk::CommandBufferBeginInfo beginInfo = {
			vk::CommandBufferUsageFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit)
		};
		commandBuffer.begin(beginInfo);

		return commandBuffer;
	}

	void Texture::_endSingleTimeCommands(vk::CommandBuffer commandBuffer) {
		vk::Device device = m_device.getNativeDevice();
		vk::Queue graphicsQueue = m_device.getGraphicsQueue();
		vk::CommandPool commandPool = m_device.getCommandPool();
		commandBuffer.end();
		vk::SubmitInfo submitInfo = { 0, nullptr, nullptr, 1, &commandBuffer, 0, nullptr };
		graphicsQueue.submit(submitInfo, nullptr);
		graphicsQueue.waitIdle();
		device.freeCommandBuffers(commandPool, commandBuffer);
	}
}