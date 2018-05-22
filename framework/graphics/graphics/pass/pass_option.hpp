#ifndef VG_PASS_OPTION_H
#define VG_PASS_OPTION_H

#include <utility>
#include <array>
#include <vulkan/vulkan.hpp>
#include <foundation/util.hpp>

namespace vg
{
	enum class ImageDescriptorType
	{
		COMBINED_IMAGE_SAMPLER,		
		INPUT_ATTACHMENT,
		BEGIN_RANGE = COMBINED_IMAGE_SAMPLER,
		END_RANGE = INPUT_ATTACHMENT,
		RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
	};

	enum class BufferDescriptorType
	{
		UNIFORM_BUFFER,
		BEGIN_RANGE = UNIFORM_BUFFER,
		END_RANGE = UNIFORM_BUFFER,
		RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
	};

	extern std::array<std::pair<ImageDescriptorType, vk::DescriptorType>, static_cast<size_t>(ImageDescriptorType::RANGE_SIZE)> arrImageDescriptorTypeToVK;
	extern std::array<std::pair<BufferDescriptorType, vk::DescriptorType>, static_cast<size_t>(ImageDescriptorType::RANGE_SIZE)> arrBufferDescriptorTypeToVK;

	extern vk::DescriptorType tranImageDescriptorTypeToVK(ImageDescriptorType type);
	extern vk::DescriptorType tranBufferDescriptorTypeToVK(BufferDescriptorType type);
}

#endif // !VG_PASS_OPTION_H
