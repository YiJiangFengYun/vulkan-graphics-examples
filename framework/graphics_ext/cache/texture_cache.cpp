#include "graphics_ext/cache/texture_cache.hpp"

namespace vge
{
     TextureCacheAllocInfo::TextureCacheAllocInfo( vk::Format format
        , uint32_t size
        , vg::Bool32 isInputUsage
        )
        : format(format)
        , size(size)
        , isInputUsage(isInputUsage)
    {

    }

    TextureCacheAllocInfo::TextureCacheAllocInfo(const TextureCacheAllocInfo &target)
        : format(target.format)
        , size(target.size)
        , isInputUsage(target.isInputUsage)
    {

    }

	TextureCacheAllocInfo& TextureCacheAllocInfo::operator=(const TextureCacheAllocInfo &target)
    {
        format = target.format;
        size = target.size;
        isInputUsage = target.isInputUsage;
		return *this;
    }

	vg::Bool32 TextureCacheAllocInfo::operator==(const TextureCacheAllocInfo &target) const
    {
        return format == target.format && size == target.size && isInputUsage == target.isInputUsage;
    }

	vg::Bool32 TextureCacheAllocInfo::operator!=(const TextureCacheAllocInfo &target) const
    {
        return ! operator==(target);
    }

    size_t TextureCacheAllocInfoHash::operator()(const TextureCacheAllocInfo& target) const
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, target.format);
        boost::hash_combine(seed, target.size);
        boost::hash_combine(seed, target.isInputUsage);
        return seed;
    }
} //vge