namespace vge
{
    template <typename TextureType> 
    TextureCache<TextureType>::TextureCache()
        : m_mapAllTextures()
        , m_mapAllTextureCapacity()
        , m_mapAllTextureCount()
        , m_mapIdleTextures()
        , m_mapIdleTextureCount()
    {

    }

    template <typename TextureType>
    std::shared_ptr<typename TextureCache<TextureType>::TextureType> 
        TextureCache<TextureType>::_createTexture(vk::Format format, uint32_t size, vg::Bool32 isInputUsage)
    {
        auto pTex = std::shared_ptr<TextureType>{
            new TextureType(format, size, size, isInputUsage)
        };
        return pTex;
    }

    template <typename TextureType>
    typename TextureCache<TextureType>::TextureType * TextureCache<TextureType>::allocateTexture(TextureCacheAllocInfo info)
    {
        TextureType *pTexture;
        if (m_mapIdleTextureCount[info] > 0) {
            pTexture = m_mapIdleTextures[info][--m_mapIdleTextureCount[info]];
        } else {
            if (m_mapAllTextureCount[info] == m_mapAllTextureCapacity[info])
            {
                uint32_t nextCapacity = getNextCapacity(m_mapAllTextureCapacity[info]);
                m_mapAllTextures[info].resize(nextCapacity);
                m_mapIdleTextures[info].resize(nextCapacity);
                m_mapAllTextureCapacity[info] = nextCapacity;
            }
            auto pNewTexture = _createTexture(info.size);
            m_mapAllTextures[info][m_mapAllTextureCount[info] ++] = pNewTexture;
            pTexture = pNewTexture.get();
        }

        return pTexture;
    }

    template <typename TextureType>
    void TextureCache<TextureType>::freeTexture(TextureCacheAllocInfo info, TextureType *pTexture)
    {
        m_mapIdleTextures[info][m_mapIdleTextureCount[info] ++] = pTexture;
    }
} //vge