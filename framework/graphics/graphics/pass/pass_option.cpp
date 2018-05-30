#include "graphics/pass/pass_option.hpp"

namespace vg
{
    std::array<std::pair<ImageDescriptorType, vk::DescriptorType>, static_cast<size_t>(ImageDescriptorType::RANGE_SIZE)> arrImageDescriptorTypeToVK = {
        std::pair<ImageDescriptorType, vk::DescriptorType>(ImageDescriptorType::COMBINED_IMAGE_SAMPLER, vk::DescriptorType::eCombinedImageSampler),
        std::pair<ImageDescriptorType, vk::DescriptorType>(ImageDescriptorType::INPUT_ATTACHMENT, vk::DescriptorType::eInputAttachment),
    };

    std::array<std::pair<BufferDescriptorType, vk::DescriptorType>, static_cast<size_t>(BufferDescriptorType::RANGE_SIZE)> arrBufferDescriptorTypeToVK = {
        std::pair<BufferDescriptorType, vk::DescriptorType>(BufferDescriptorType::UNIFORM_BUFFER, vk::DescriptorType::eUniformBuffer),
    };

    vk::DescriptorType tranImageDescriptorTypeToVK(ImageDescriptorType type)
    {
        vk::DescriptorType vkType;
#ifdef DEBUG
        Bool32 isHave = VG_FALSE;
#endif // DEBUG
        for (const auto& item2 : arrImageDescriptorTypeToVK)
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

    vk::DescriptorType tranBufferDescriptorTypeToVK(BufferDescriptorType type)
    {
        vk::DescriptorType vkType;
#ifdef DEBUG
        Bool32 isHave = VG_FALSE;
#endif // DEBUG
        for (const auto& item2 : arrBufferDescriptorTypeToVK)
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