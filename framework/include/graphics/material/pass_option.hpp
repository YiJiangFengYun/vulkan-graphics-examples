#ifndef KGS_PASS_OPTION_H
#define KGS_PASS_OPTION_H

#include <utility>
#include <array>
#include <vulkan/vulkan.hpp>
#include <foundation/util.hpp>

namespace kgs
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

	using ShaderStageFlags = fd::Flags<ShaderStageFlagBits>;

	enum class CullModeFlagBits
	{
		eNone = 0u ,
		eFront = 1u,
		eBack = 2u,
		eFrontAndBack = 3u
	};

	const uint32_t CullModeFlagCount = 4u;

	using CullModeFlags = fd::Flags<CullModeFlagBits>;

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
		Bool32 isHave = KGS_FALSE;
#endif // DEBUG
		for (const auto& item2 : arrFrontFaceTypeToVK)
		{
			if (item2.first == type)
			{
				vkType = item2.second;
#ifdef DEBUG
				isHave = KGS_TRUE;
#endif // DEBUG
				break;
			}
		}

#ifdef DEBUG
		if (isHave == KGS_FALSE)
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
		Bool32 isHave = KGS_FALSE;
#endif // DEBUG
		for (const auto& item2 : arrDescriptorTypeToVK)
		{
			if (item2.first == type)
			{
				vkType = item2.second;
#ifdef DEBUG
				isHave = KGS_TRUE;
#endif // DEBUG
				break;
			}
		}

#ifdef DEBUG
		if (isHave == KGS_FALSE)
		{
			throw std::runtime_error("Invalid descriptor type ");
		}
#endif // DEBUG
		return vkType;
	}
}

#endif // !KGS_PASS_OPTION_H
