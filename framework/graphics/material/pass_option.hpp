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
		BEGIN_RANGE = COMBINED_IMAGE_SAMPLER,
		END_RANGE = UNIFORM_BUFFER,
		RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
	};

	enum class ShaderStageFlagBits
	{
		VERTEX = 1u,
		FRAGMENT = 2u
	};

	const uint32_t ShaderStageFlagCount = 2u;

	typedef fd::Flags<ShaderStageFlagBits> ShaderStageFlags;

	enum class CullModeFlagBits
	{
		eNone = 0u ,
		eFront = 1u,
		eBack = 2u,
		eFrontAndBack = 3u
	};

	const uint32_t CullModeFlagCount = 4u;

	typedef fd::Flags<CullModeFlagBits> CullModeFlags;

	enum class FrontFaceType
	{
		eCounterClockwise = 0u,
		eClockwise = 1u,
		BEGIN_RANGE = eCounterClockwise,
		END_RANGE = eClockwise,
		RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
	};

	extern std::array<std::pair<DescriptorType, vk::DescriptorType>, static_cast<size_t>(DescriptorType::RANGE_SIZE)> arrDescriptorTypeToVK;
	extern std::array<std::pair<ShaderStageFlagBits, vk::ShaderStageFlagBits>, static_cast<size_t>(ShaderStageFlagCount)> arrShaderStageFlagBitsToVK;
	extern std::array<std::pair<CullModeFlagBits, vk::CullModeFlagBits>, static_cast<size_t>(CullModeFlagCount)> arrCullModeFlagBitsToVK;
	extern std::array<std::pair<FrontFaceType, vk::FrontFace>, static_cast<size_t>(FrontFaceType::RANGE_SIZE)> arrFrontFaceTypeToVK;

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
			throw std::runtime_error("Invalid descriptor type ");
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
