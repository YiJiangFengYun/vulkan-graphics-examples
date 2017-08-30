#include "graphics/texture/texture.hpp"

namespace kgs
{
	Texture::Texture(Device device, TextureFormat format, Bool32 mipMap)
		:m_device(device),
		m_physicalDevice(device.getPhysicalDevice()),
		m_nativeDevice(device.getNativeDevice()),
		m_format(format),
		m_mipMap(mipMap)
	{
	}

	Texture::~Texture()
	{
		_destroySampler();
		_destroyImageView();
		_destroyImage();
	}

	void Texture::_createImage(TextureType type, TextureFormat format, uint32_t width, uint32_t height, uint32_t depth, uint32_t arrayLength)
	{
#ifdef DEBUG
		Bool32 isFind;
#endif // DEBUG

		vk::Format vkFormat;
		for (const auto& item : arrFormatToVKFormat)
		{
			if (item.first == format)
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

		vk::ImageType vkImageType;
#ifdef DEBUG
		isFind = KGS_FALSE;
#endif // DEBUG

		for (const auto& item : arrTextureTypeToVKImageType)
		{
			if (item.first == type)
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
		switch (type)
		{
		case TextureType::TEX_1D:
		case TextureType::TEX_1D_ARRAY:
		{
			if (width == 0) throw std::invalid_argument("Invalid width argument at creating image for texture.");
			if (height != 1) throw std::invalid_argument("Invalid height argument at creating image for texture.");
			if (depth != 1) throw std::invalid_argument("Invalide depth argument at creating image for texture.");
			break;
		}
		case TextureType::TEX_2D:
		case TextureType::TEX_2D_ARRAY:
		{
			if (width == 0) throw std::invalid_argument("Invalid width argument at creating image for texture.");
			if (height == 0) throw std::invalid_argument("Invalid height argument at creating image for texture.");
			if (depth != 1) throw std::invalid_argument("Invalide depth argument at creating image for texture.");
			break;
		}
		case TextureType::CUBE:
		case TextureType::CUBE_ARRARY:
		{
			if (width == 0) throw std::invalid_argument("Invalid width argument at creating image for texture.");
			if (width != height) throw std::invalid_argument("Invalid width and height argument at creating image for texture, because they is not equal.");
			if (depth != 1) throw std::invalid_argument("Invalide depth argument at creating image for texture.");
			break;
		}
		case TextureType::TEX_3D:
		{
			if (width == 0) throw std::invalid_argument("Invalid width argument at creating image for texture.");
			if (height == 0) throw std::invalid_argument("Invalid height argument at creating image for texture.");
			if (depth == 0) throw std::invalid_argument("Invalid depth argument at creating image for texture.");
			break;
		}
		default:
			throw std::logic_error("Lack of checking argument at creating iamge for texture when texture type is " + mapTextureTypeToName[type]);
			break;
		}

#endif // DEBUG

		//caculate compatible array layer and flag.
		uint32_t arraylayer;
		vk::ImageCreateFlags flags;
		switch (type)
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
			arraylayer = arrayLength;
			break;
		}
		case TextureType::CUBE:
		{
			arraylayer = 6U;
			flags |= vk::ImageCreateFlagBits::eCubeCompatible;
			break;
		}
		case TextureType::CUBE_ARRARY:
		{
			arraylayer = 6U * arrayLength;
			flags |= vk::ImageCreateFlagBits::eCubeCompatible;
			break;
		}
		default:
			throw std::logic_error("Lack of caculating compatible array layer and flag at creating iamge for texture when texture type is " + mapTextureTypeToName[type]);
			break;
		}


		vk::ImageCreateInfo createInfo = {
			flags,
			vkImageType,
			vkFormat,
			{
				width,
				height,
				depth
			},
			//todo mipMap 
			1U,
			arrayLength,
			vk::SampleCountFlagBits::e1,
			vk::ImageTiling::eOptimal,
			vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
			vk::SharingMode::eExclusive,
			0U,
			nullptr,
			vk::ImageLayout::eUndefined
		};

		m_image = m_nativeDevice.createImage(createInfo, nullptr);

		auto memRequirements = m_nativeDevice.getImageMemoryRequirements(m_image);

		vk::MemoryAllocateInfo allocInfo = {
			memRequirements.size,
			_findMemoryType(memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal)
		};

		m_memory = m_nativeDevice.allocateMemory(allocInfo, nullptr);

		m_nativeDevice.bindImageMemory(m_image, m_memory, vk::DeviceSize(0));
	}

	void Texture::_destroyImage()
	{
		m_nativeDevice.destroyImage(m_image);
		m_nativeDevice.freeMemory(m_memory);
	}


	void Texture::_createImageView(TextureType type, TextureFormat format, int32_t arrayLength)
	{

		vk::Format vkFormat;
		for (const auto& item : arrFormatToVKFormat)
		{
			if (item.first == format)
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
			if (item.first == type)
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

		//caculate compatible layer count
		uint32_t layerCount;
		switch (type)
		{
		case TextureType::TEX_1D:
		case TextureType::TEX_2D:
		case TextureType::TEX_3D:
		{
			layerCount = 1;
			break;
		}
		case TextureType::TEX_1D_ARRAY:
		case TextureType::TEX_2D_ARRAY:
		{
			layerCount = arrayLength;
			break;
		}
		case TextureType::CUBE:
		{
			layerCount = 6;
			break;
		}
		case TextureType::CUBE_ARRARY:
		{
			layerCount = 6 * arrayLength;
			break;
		}
		default:
			throw std::logic_error("Lack of caculating compatible layer count at creating iamge view for texture when texture type is " + mapTextureTypeToName[type]);
			break;
		}

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
				//todo mipMap 
				uint32_t(1),
				uint32_t(0),
				layerCount
			}
		};

		m_imageView = m_nativeDevice.createImageView(createInfo);
	}

	void Texture::_destroyImageView()
	{
		m_nativeDevice.destroyImageView(m_imageView);
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

		m_sampler = m_nativeDevice.createSampler(createInfo);
	}

	void Texture::_destroySampler()
	{
		m_nativeDevice.destroySampler(m_sampler);
	}

	uint32_t Texture::_findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties)
	{
		vk::PhysicalDeviceMemoryProperties memProperties = m_physicalDevice.getMemoryProperties();
		for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
		{
			if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}

		throw std::runtime_error("Failed to find suitable memory type!");

	}
}