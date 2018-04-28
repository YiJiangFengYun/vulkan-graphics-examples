#include "graphics_ext/cache/texture_cache.hpp"

namespace vge
{
     TextureCacheAllocInfo::TextureCacheAllocInfo( vk::Format format
        , uint32_t width
        , uint32_t height
        , vg::Bool32 isInputUsage
        )
        : format(format)
        , width(width)
        , height(height)
        , isInputUsage(isInputUsage)
    {

    }

    TextureCacheAllocInfo::TextureCacheAllocInfo(const TextureCacheAllocInfo &target)
        : format(target.format)
        , width(target.width)
        , height(target.height)
        , isInputUsage(target.isInputUsage)
    {

    }

	TextureCacheAllocInfo& TextureCacheAllocInfo::operator=(const TextureCacheAllocInfo &target)
    {
        format = target.format;
        width = target.width;
        height = target.height;
        isInputUsage = target.isInputUsage;
		return *this;
    }

	vg::Bool32 TextureCacheAllocInfo::operator==(const TextureCacheAllocInfo &target) const
    {
        return format == target.format && width == target.width && height == target.height && isInputUsage == target.isInputUsage;
    }

	vg::Bool32 TextureCacheAllocInfo::operator!=(const TextureCacheAllocInfo &target) const
    {
        return ! operator==(target);
    }

    size_t TextureCacheAllocInfoHash::operator()(const TextureCacheAllocInfo& target) const
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, target.format);
        boost::hash_combine(seed, target.width);
        boost::hash_combine(seed, target.height);
        boost::hash_combine(seed, target.isInputUsage);
        return seed;
    }
} //vge