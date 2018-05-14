#include "graphics/texture/texture_default.hpp"

namespace vg
{
   std::shared_ptr<Texture2D> pDefaultTexture2D = nullptr;

    void createDefaultTexture()
    {
        pDefaultTexture2D = std::shared_ptr<Texture2D>{
           new Texture2D(vk::Format::eR8G8B8A8Unorm, VG_FALSE, 1u, 1u)
           };
        
        Color32 content = Color32(255);
        TextureDataInfo::Component component = {
            0,
            0u,
            1u,
            static_cast<uint32_t>(sizeof(content)),
            VG_TRUE,
            1u,
            1u,
            1u,
        };
        TextureDataInfo info = {
            1u,
            &component
        };
        pDefaultTexture2D->applyData(info, &content, static_cast<uint32_t>(sizeof(content)));
    }

    void destroyDefaultTexture()
    {
        pDefaultTexture2D = nullptr;
    }
} //vg