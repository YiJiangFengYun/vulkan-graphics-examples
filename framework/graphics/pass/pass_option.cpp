#include "graphics/pass/pass_option.hpp"

namespace vg
{
	std::array<std::pair<DescriptorType, vk::DescriptorType>, static_cast<size_t>(DescriptorType::RANGE_SIZE)> arrDescriptorTypeToVK = {
		std::pair<DescriptorType, vk::DescriptorType>(DescriptorType::COMBINED_IMAGE_SAMPLER, vk::DescriptorType::eCombinedImageSampler),
		std::pair<DescriptorType, vk::DescriptorType>(DescriptorType::UNIFORM_BUFFER, vk::DescriptorType::eUniformBuffer)
	};

	std::array<std::pair<ShaderStageFlagBits, vk::ShaderStageFlagBits>, static_cast<size_t>(ShaderStageFlagCount)> arrShaderStageFlagBitsToVK = {
		std::pair<ShaderStageFlagBits, vk::ShaderStageFlagBits>(ShaderStageFlagBits::VERTEX, vk::ShaderStageFlagBits::eVertex),
		std::pair<ShaderStageFlagBits, vk::ShaderStageFlagBits>(ShaderStageFlagBits::FRAGMENT, vk::ShaderStageFlagBits::eFragment)
	};

	std::array<std::pair<CullModeFlagBits, vk::CullModeFlagBits>, static_cast<size_t>(CullModeFlagCount)> arrCullModeFlagBitsToVK = {
		std::pair<CullModeFlagBits, vk::CullModeFlagBits>(CullModeFlagBits::NONE, vk::CullModeFlagBits::eNone),
		std::pair<CullModeFlagBits, vk::CullModeFlagBits>(CullModeFlagBits::FRONT, vk::CullModeFlagBits::eFront),
		std::pair<CullModeFlagBits, vk::CullModeFlagBits>(CullModeFlagBits::BACK, vk::CullModeFlagBits::eBack),
		std::pair<CullModeFlagBits, vk::CullModeFlagBits>(CullModeFlagBits::FRONT_AND_BACK, vk::CullModeFlagBits::eFrontAndBack)
	};

	std::array<std::pair<FrontFaceType, vk::FrontFace>, static_cast<size_t>(FrontFaceType::RANGE_SIZE)> arrFrontFaceTypeToVK = {
		std::pair<FrontFaceType, vk::FrontFace>(FrontFaceType::COUNTER_CLOCKWISE, vk::FrontFace::eCounterClockwise),
		std::pair<FrontFaceType, vk::FrontFace>(FrontFaceType::CLOCKWISE, vk::FrontFace::eClockwise)
	};

	std::array<std::pair<PolygonMode, vk::PolygonMode>, static_cast<size_t>(PolygonMode::RANGE_SIZE)> arrPolygonModeToVK = {
		std::pair<PolygonMode, vk::PolygonMode>(PolygonMode::FILL, vk::PolygonMode::eFill),
		std::pair<PolygonMode, vk::PolygonMode>(PolygonMode::LINE, vk::PolygonMode::eLine),
		std::pair<PolygonMode, vk::PolygonMode>(PolygonMode::POINT, vk::PolygonMode::ePoint)
	};
}