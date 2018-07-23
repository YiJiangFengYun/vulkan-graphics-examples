#include "graphics/texture/texture_default.hpp"

namespace vg
{
    std::shared_ptr<Texture1D> pDefaultTexture1D = nullptr;
    std::shared_ptr<Texture2D> pDefaultTexture2D = nullptr;
    std::shared_ptr<Texture3D> pDefaultTexture3D = nullptr;
    std::shared_ptr<TextureCube> pDefaultTextureCube = nullptr;
    std::shared_ptr<Texture1DArray> pDefaultTexture1DArray = nullptr;
    std::shared_ptr<Texture2DArray> pDefaultTexture2DArray = nullptr;
    std::shared_ptr<TextureCubeArray> pDefaultTextureCubeArray = nullptr;

    std::array<std::pair<vk::ImageViewType, const Texture *>, 7u> arrImageViewTypeToDefaultTexture = {
        std::pair<vk::ImageViewType, const Texture *>{vk::ImageViewType::e1D, nullptr},
        std::pair<vk::ImageViewType, const Texture *>{vk::ImageViewType::e2D, nullptr},
        std::pair<vk::ImageViewType, const Texture *>{vk::ImageViewType::e3D, nullptr},
        std::pair<vk::ImageViewType, const Texture *>{vk::ImageViewType::eCube, nullptr},
        std::pair<vk::ImageViewType, const Texture *>{vk::ImageViewType::e1DArray, nullptr},
        std::pair<vk::ImageViewType, const Texture *>{vk::ImageViewType::e2DArray, nullptr},
        std::pair<vk::ImageViewType, const Texture *>{vk::ImageViewType::eCubeArray, nullptr},
    };

    const Texture *getDefaultTexture(vk::ImageViewType type)
    {
        return arrImageViewTypeToDefaultTexture[static_cast<size_t>(type)].second;
    }

    void createDefaultTextures()
    {
        //default textures: 1D, 2D, 3D, 1D array, 2D array
        {
            Color32 content = Color32(255);
            TextureDataInfo::Component component = {
                0u,
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

            //1D tex
            pDefaultTexture1D = std::shared_ptr<Texture1D>{
                new Texture1D(vk::Format::eR8G8B8A8Unorm, VG_FALSE, 1u)
                };
            
            pDefaultTexture1D->applyData(info, &content, static_cast<uint32_t>(sizeof(content)));

            //2D tex
            pDefaultTexture2D = std::shared_ptr<Texture2D>{
                new Texture2D(vk::Format::eR8G8B8A8Unorm, VG_FALSE, 1u, 1u)
                };

            pDefaultTexture2D->applyData(info, &content, static_cast<uint32_t>(sizeof(content)));

            //3D tex
            pDefaultTexture3D = std::shared_ptr<Texture3D>{
                new Texture3D(vk::Format::eR8G8B8A8Unorm, VG_FALSE, 1u, 1u, 1u)
                };
            
            pDefaultTexture3D->applyData(info, &content, static_cast<uint32_t>(sizeof(content)));

            //1D array tex
            pDefaultTexture1DArray = std::shared_ptr<Texture1DArray>{
                new Texture1DArray(vk::Format::eR8G8B8A8Unorm, VG_FALSE, 1u, 1u)
                };
            
            pDefaultTexture1DArray->applyData(info, &content, static_cast<uint32_t>(sizeof(content)));

            //2D array tex
            pDefaultTexture2DArray = std::shared_ptr<Texture2DArray>{
                new Texture2DArray(vk::Format::eR8G8B8A8Unorm, VG_FALSE, 1u, 1u, 1u)
                };

            pDefaultTexture2DArray->applyData(info, &content, static_cast<uint32_t>(sizeof(content)));
        }

        //Default textures: cube, cube array.
        {

            Color32 content[6] = {
                Color32(255), 
                Color32(255), 
                Color32(255), 
                Color32(255), 
                Color32(255), 
                Color32(255), 
            };

            TextureDataInfo::Component component = {
                0u,
                0u,
                6u,
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

            //Cube tex
            pDefaultTextureCube = std::shared_ptr<TextureCube>{
                new TextureCube(vk::Format::eR8G8B8A8Unorm, VG_FALSE, 1u, 1u)
                };
            
            pDefaultTextureCube->applyData(info, &content, static_cast<uint32_t>(sizeof(content)));

            //Cube array tex
            auto deviceFeatures = pApp->getPhysicalDeviceFeatures();
            if (deviceFeatures.imageCubeArray == VK_TRUE) {
                pDefaultTextureCubeArray = std::shared_ptr<TextureCubeArray>{
                    new TextureCubeArray(vk::Format::eR8G8B8A8Unorm, VG_FALSE, 1u, 1u, 1u)
                    };
                
                pDefaultTextureCubeArray->applyData(info, &content, static_cast<uint32_t>(sizeof(content)));
            }
        }


        arrImageViewTypeToDefaultTexture[static_cast<size_t>(vk::ImageViewType::e1D)].second = pDefaultTexture1D.get();
        arrImageViewTypeToDefaultTexture[static_cast<size_t>(vk::ImageViewType::e2D)].second = pDefaultTexture2D.get();
        arrImageViewTypeToDefaultTexture[static_cast<size_t>(vk::ImageViewType::e3D)].second = pDefaultTexture3D.get();
        arrImageViewTypeToDefaultTexture[static_cast<size_t>(vk::ImageViewType::eCube)].second = pDefaultTextureCube.get();
        arrImageViewTypeToDefaultTexture[static_cast<size_t>(vk::ImageViewType::e1DArray)].second = pDefaultTexture1DArray.get();
        arrImageViewTypeToDefaultTexture[static_cast<size_t>(vk::ImageViewType::e2DArray)].second = pDefaultTexture2DArray.get();
        arrImageViewTypeToDefaultTexture[static_cast<size_t>(vk::ImageViewType::eCubeArray)].second = pDefaultTextureCubeArray.get();
    }

    void destroyDefaultTextures()
    {
        arrImageViewTypeToDefaultTexture[static_cast<size_t>(vk::ImageViewType::e1D)].second = nullptr;
        arrImageViewTypeToDefaultTexture[static_cast<size_t>(vk::ImageViewType::e2D)].second = nullptr;
        arrImageViewTypeToDefaultTexture[static_cast<size_t>(vk::ImageViewType::e3D)].second = nullptr;
        arrImageViewTypeToDefaultTexture[static_cast<size_t>(vk::ImageViewType::eCube)].second = nullptr;
        arrImageViewTypeToDefaultTexture[static_cast<size_t>(vk::ImageViewType::e1DArray)].second = nullptr;
        arrImageViewTypeToDefaultTexture[static_cast<size_t>(vk::ImageViewType::e2DArray)].second = nullptr;
        arrImageViewTypeToDefaultTexture[static_cast<size_t>(vk::ImageViewType::eCubeArray)].second = nullptr;

        pDefaultTexture1D = nullptr;
        pDefaultTexture2D = nullptr;
        pDefaultTexture3D = nullptr;
        pDefaultTextureCube = nullptr;
        pDefaultTexture1DArray = nullptr;
        pDefaultTexture2DArray = nullptr;
        pDefaultTextureCubeArray = nullptr;
    }
} //vg