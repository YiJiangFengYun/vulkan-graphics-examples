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

	extern std::array<std::pair<DescriptorType, vk::DescriptorType>, static_cast<size_t>(DescriptorType::RANGE_SIZE)> arrDescriptorTypeToVK;
	extern std::array<std::pair<ShaderStageFlagBits, vk::ShaderStageFlagBits>, static_cast<size_t>(ShaderStageFlagCount)> arrShaderStageFlagBitsToVK;

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
