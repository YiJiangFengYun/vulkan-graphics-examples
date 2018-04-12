#ifndef VG_PASS_OPTION_H
#define VG_PASS_OPTION_H

#include <utility>
#include <array>
#include <vulkan/vulkan.hpp>
#include <foundation/util.hpp>

namespace vg
{
	enum class DescriptorType
	{
		COMBINED_IMAGE_SAMPLER,
		UNIFORM_BUFFER,
		INPUT_ATTACHMENT,
		BEGIN_RANGE = COMBINED_IMAGE_SAMPLER,
		END_RANGE = INPUT_ATTACHMENT,
		RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
	};

	enum class ShaderStageFlagBits
	{
		VERTEX = 1u,
		FRAGMENT = 2u
	};

	const uint32_t ShaderStageFlagCount = 2u;

	using ShaderStageFlags = fd::Flags<ShaderStageFlagBits>;

	enum class PolygonMode
	{
		FILL = VK_POLYGON_MODE_FILL,
        LINE = VK_POLYGON_MODE_LINE,
        POINT = VK_POLYGON_MODE_POINT,
		BEGIN_RANGE = FILL,
		END_RANGE = POINT,
		RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
	};

	enum class CullModeFlagBits
	{
		NONE = 0u ,
		FRONT = 1u,
		BACK = 2u,
		FRONT_AND_BACK = 3u
	};

	const uint32_t CullModeFlagCount = 4u;

	using CullModeFlags = fd::Flags<CullModeFlagBits>;

	enum class FrontFaceType
	{
		COUNTER_CLOCKWISE = 0u,
		CLOCKWISE = 1u,
		BEGIN_RANGE = COUNTER_CLOCKWISE,
		END_RANGE = CLOCKWISE,
		RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
	};

	extern std::array<std::pair<DescriptorType, vk::DescriptorType>, static_cast<size_t>(DescriptorType::RANGE_SIZE)> arrDescriptorTypeToVK;
	extern std::array<std::pair<ShaderStageFlagBits, vk::ShaderStageFlagBits>, static_cast<size_t>(ShaderStageFlagCount)> arrShaderStageFlagBitsToVK;
	extern std::array<std::pair<CullModeFlagBits, vk::CullModeFlagBits>, static_cast<size_t>(CullModeFlagCount)> arrCullModeFlagBitsToVK;
	extern std::array<std::pair<FrontFaceType, vk::FrontFace>, static_cast<size_t>(FrontFaceType::RANGE_SIZE)> arrFrontFaceTypeToVK;
	extern std::array<std::pair<PolygonMode, vk::PolygonMode>, static_cast<size_t>(PolygonMode::RANGE_SIZE)> arrPolygonModeToVK;

	inline vk::PolygonMode tranPolygonModeToVK(PolygonMode mode)
	{
		vk::PolygonMode vkMode;
#ifdef DEBUG
		Bool32 isHave = VG_FALSE;
#endif // DEBUG
		for (const auto& item2 : arrPolygonModeToVK)
		{
			if (item2.first == mode)
			{
				vkMode = item2.second;
#ifdef DEBUG
				isHave = VG_TRUE;
#endif // DEBUG
				break;
			}
		}

#ifdef DEBUG
		if (isHave == VG_FALSE)
		{
			throw std::runtime_error("Invalid polygon mode.");
		}
#endif // DEBUG
		return vkMode;
	}

	inline vk::FrontFace tranFrontFaceTypeToVK(FrontFaceType type)
	{
		vk::FrontFace vkType;
#ifdef DEBUG
		Bool32 isHave = VG_FALSE;
#endif // DEBUG
		for (const auto& item2 : arrFrontFaceTypeToVK)
		{
			if (item2.first == type)
			{
				vkType = item2.second;
#ifdef DEBUG
				isHave = VG_TRUE;
#endif // DEBUG
				break;
			}
		}

#ifdef DEBUG
		if (isHave == VG_FALSE)
		{
			throw std::runtime_error("Invalid front face type. ");
		}
#endif // DEBUG
		return vkType;
	}

	inline vk::CullModeFlags tranCullModeFlagsToVK(CullModeFlags flags)
	{
		size_t size = arrCullModeFlagBitsToVK.size();
		vk::CullModeFlags result;

		for (uint32_t i = 0; i < static_cast<uint32_t>(CullModeFlagCount); ++i)
		{
			uint32_t flag = 1 << i;
			if (flags & static_cast<CullModeFlagBits>(flag))
			{
				for (uint32_t j = 0; j < size; ++j)
				{
					if (arrCullModeFlagBitsToVK[j].first == static_cast<CullModeFlagBits>(flag))
					{
						result |= arrCullModeFlagBitsToVK[j].second;
						break;
					}
				}
			}
		}

		return result;
	}

	inline vk::ShaderStageFlagBits tranShaderStageFlagBitToVK(ShaderStageFlagBits flagBit) 
	{
		size_t size = arrShaderStageFlagBitsToVK.size();
		for (uint32_t i = 0; i < size; ++i)
		{
			if (arrShaderStageFlagBitsToVK[i].first == flagBit)
			{
				return arrShaderStageFlagBitsToVK[i].second;
			}
		}
		throw std::invalid_argument("Error to transform vg::ShaderStageFlagBit to vk.");
	}

	inline vk::ShaderStageFlags tranShaderStageFlagsToVK(ShaderStageFlags flags)
	{
		size_t size = arrShaderStageFlagBitsToVK.size();
		vk::ShaderStageFlags result;

		for (uint32_t i = 0; i < static_cast<uint32_t>(ShaderStageFlagCount); ++i)
		{
			uint32_t flag = 1 << i;
			if (flags & static_cast<ShaderStageFlagBits>(flag))
			{
				for (uint32_t j = 0; j < size; ++j)
				{
					if (arrShaderStageFlagBitsToVK[j].first == static_cast<ShaderStageFlagBits>(flag))
					{
						result |= arrShaderStageFlagBitsToVK[j].second;
						break;
					}
				}
			}
		}

		return result;
	}

	inline vk::DescriptorType tranDescriptorTypeToVK(DescriptorType type)
	{
		vk::DescriptorType vkType;
#ifdef DEBUG
		Bool32 isHave = VG_FALSE;
#endif // DEBUG
		for (const auto& item2 : arrDescriptorTypeToVK)
		{
			if (item2.first == type)
			{
				vkType = item2.second;
#ifdef DEBUG
				isHave = VG_TRUE;
#endif // DEBUG
				break;
			}
		}

#ifdef DEBUG
		if (isHave == VG_FALSE)
		{
			throw std::runtime_error("Invalid descriptor type ");
		}
#endif // DEBUG
		return vkType;
	}
}

#endif // !VG_PASS_OPTION_H
