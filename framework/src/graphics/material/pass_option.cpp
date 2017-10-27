#include "graphics/material/pass_option.hpp"

namespace kgs
{
	std::array<std::pair<DescriptorType, vk::DescriptorType>, static_cast<size_t>(DescriptorType::RANGE_SIZE)> arrDescriptorTypeToVK = {
		std::pair<DescriptorType, vk::DescriptorType>(DescriptorType::COMBINED_IMAGE_SAMPLER, vk::DescriptorType::eCombinedImageSampler),
		std::pair<DescriptorType, vk::DescriptorType>(DescriptorType::UNIFORM_BUFFER, vk::DescriptorType::eUniformBuffer)
	};

	std::array<std::pair<ShaderStageFlagBits, vk::ShaderStageFlagBits>, static_cast<size_t>(ShaderStageFlagCount)> arrShaderStageFlagBitsToVK = {
		std::pair<ShaderStageFlagBits, vk::ShaderStageFlagBits>(ShaderStageFlagBits::VERTEX, vk::ShaderStageFlagBits::eVertex),
		std::pair<ShaderStageFlagBits, vk::ShaderStageFlagBits>(ShaderStageFlagBits::FRAGMENT, vk::ShaderStageFlagBits::eFragment)
	};
}