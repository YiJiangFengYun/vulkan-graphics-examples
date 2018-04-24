#ifndef VGE_TEXTURE_CACHE_HPP
#define VGE_TEXTURE_CACHE_HPP

#include "graphics_ext/global.hpp"

namespace vge
{
    struct TextureCacheAllocInfo {
        vk::Format format;
        uint32_t size;
        vg::Bool32 isInputUsage;

        TextureCacheAllocInfo( vk::Format format = vk::Format::eUndefined
            , uint32_t size = 0u
            , vg::Bool32 isInputUsage = VG_FALSE
            );
        TextureCacheAllocInfo(const TextureCacheAllocInfo &);
		TextureCacheAllocInfo& operator=(const TextureCacheAllocInfo &);
		vg::Bool32 operator==(const TextureCacheAllocInfo &) const;
		vg::Bool32 operator!=(const TextureCacheAllocInfo &) const;
    };

    struct TextureCacheAllocInfoHash {
        size_t operator()(const TextureCacheAllocInfo&) const;
    };

    template <typename TexType>
    class TextureCache 
    {
    public:
		using TextureType = TexType;
        
        TextureCache();
        TextureType *allocateTexture(TextureCacheAllocInfo info);
        void freeTexture(TextureCacheAllocInfo info, TextureType *pTexture);

    private:
        
        std::unordered_map<TextureCacheAllocInfo, std::vector<std::shared_ptr<TextureType>>, TextureCacheAllocInfoHash> m_mapAllTextures;
        std::unordered_map<TextureCacheAllocInfo, uint32_t, TextureCacheAllocInfoHash> m_mapAllTextureCapacity;
        std::unordered_map<TextureCacheAllocInfo, uint32_t, TextureCacheAllocInfoHash> m_mapAllTextureCount;
        std::unordered_map<TextureCacheAllocInfo, std::vector<TextureType *>, TextureCacheAllocInfoHash> m_mapIdleTextures;
        std::unordered_map<TextureCacheAllocInfo, uint32_t, TextureCacheAllocInfoHash> m_mapIdleTextureCount;

        std::shared_ptr<TextureType> _createTexture(vk::Format format, uint32_t size, vg::Bool32 isInputUsage);
    };
} //vge

#include "graphics_ext/cache/texture_cache.inl"

#endif //VGE_TEXTURE_CACHE_HPP