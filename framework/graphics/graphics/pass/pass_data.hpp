#ifndef VG_PASS_DATA_HPP
#define VG_PASS_DATA_HPP

#include "graphics/global.hpp"
#include "graphics/buffer_data/buffer_data.hpp"
#include "graphics/buffer_data/uniform_buffer_data.hpp"
#include "graphics/texture/texture.hpp"
#include "graphics/pass/pass_option.hpp"


#define VG_PASS_BUILDIN_DATA_NAME "_BuildIn"

#define VG_PASS_PRE_DEPTH_TEXTURE_NAME "_pre_depth_depth_tex"
#define VG_PASS_POST_RENDER_TEXTURE_NAME "_post_render_tex"
#define VG_PASS_LIGHT_DATA_BUFFER_NAME "_light_data_buffer"
#define VG_PASS_LIGHT_TEXTURE_NAME "_light_texture"
#define VG_PASS_LIGHT_RENDER_DATA_NAME "_light_render_data"

#define VG_PASS_BUILDIN_DATA_LAYOUT_PRIORITY 0
#define VG_PASS_LIGHT_RENDER_DATA_LAYOUT_PRIORITY 1
#define VG_PASS_OTHER_DATA_MIN_LAYOUT_PRIORITY 2

#define VG_PASS_PRE_DEPTH_TEXTURE_BINDING_PRIORITY 0
#define VG_PASS_POST_RENDER_TEXTURE_BINDING_PRIORITY 1
#define VG_PASS_LIGHT_DATA_BUFFER_BINDING_PRIORITY 2
#define VG_PASS_LIGHT_TEXTURE_MIN_BINDING_PRIORITY 3
#define VG_PASS_OTHER_MIN_BINDING_PRIORITY 100

namespace vg
{
    struct PassDataInfo {
        uint32_t layoutPriority;
        vk::ShaderStageFlags shaderStageFlags;
        PassDataInfo(uint32_t layoutPriority = 0u
            , vk::ShaderStageFlags shaderStageFlags = vk::ShaderStageFlags()
        );
        PassDataInfo(const PassDataInfo &);
        PassDataInfo& operator=(const PassDataInfo &);
        
    };

    struct PassDataSizeInfo {
        uint32_t size;

        PassDataSizeInfo(uint32_t size = 0u);
        PassDataSizeInfo(const PassDataSizeInfo &);
        PassDataSizeInfo& operator=(const PassDataSizeInfo &);
        uint32_t getBufferSize() const;
    };

    struct PassTextureInfo {
        struct TextureInfo {
            const Texture *pTexture;
            const Texture::ImageView *pImageView;
            const Texture::Sampler *pSampler;
            vk::ImageLayout imageLayout;
            TextureInfo(const Texture *pTexture = nullptr
                , const Texture::ImageView *pImageView = nullptr
                , const Texture::Sampler *pSampler = nullptr
                , vk::ImageLayout imageLayout = vk::ImageLayout::eUndefined
            );
            TextureInfo(const TextureInfo &);
            TextureInfo& operator=(const TextureInfo &);
        };
        SamplerTextureType textureType;
        uint32_t textureCount;
        const TextureInfo *pTextures;
        uint32_t bindingPriority;
        ImageDescriptorType descriptorType;
        vk::ShaderStageFlags stageFlags;
        PassTextureInfo(SamplerTextureType textureType = SamplerTextureType::TEX_1D
            , uint32_t textureCount = 0u
            , const TextureInfo *pTextures = nullptr
            , uint32_t bindingPriority = 0u
            , ImageDescriptorType descriptorType = ImageDescriptorType::COMBINED_IMAGE_SAMPLER
            , vk::ShaderStageFlags stageFlags = vk::ShaderStageFlagBits::eFragment
            );
        PassTextureInfo(const PassTextureInfo &);
        PassTextureInfo& operator=(const PassTextureInfo &);
    };

    struct PassTextureData {
        SamplerTextureType textureType;
        std::vector<PassTextureInfo::TextureInfo> textures;
        uint32_t bindingPriority;
        ImageDescriptorType descriptorType;
        vk::ShaderStageFlags stageFlags;
        PassTextureData(SamplerTextureType textureType = SamplerTextureType::TEX_1D
            , std::vector<PassTextureInfo::TextureInfo> textures = {}
            , uint32_t bindingPriority = 0u
            , ImageDescriptorType descriptorType = ImageDescriptorType::COMBINED_IMAGE_SAMPLER
            , vk::ShaderStageFlags stageFlags = vk::ShaderStageFlagBits::eFragment
            );
        PassTextureData(const PassTextureData &);
        PassTextureData(const PassTextureInfo &);
        PassTextureData& operator=(const PassTextureData &);
        PassTextureData& operator=(const PassTextureInfo &);
        PassTextureInfo getTextureInfo() const;
    };

    struct PassBufferInfo {
        struct BufferInfo {
            const BufferData *pBuffer;
            uint32_t offset;
            uint32_t range;
            BufferInfo(const BufferData *pBuffer = nullptr
                , uint32_t offset = 0u
                , uint32_t range = 0u
            );
            BufferInfo(const BufferInfo &);
            BufferInfo& operator=(const BufferInfo &);
        };
        uint32_t bufferCount;
        const BufferInfo *pBuffers;
        uint32_t bindingPriority;
        BufferDescriptorType descriptorType;
        vk::ShaderStageFlags stageFlags;
        PassBufferInfo(uint32_t bufferCount = 0u
            , const BufferInfo *pBuffers = nullptr
            , uint32_t bindingPriority = 0u
            , BufferDescriptorType descriptorType = BufferDescriptorType::UNIFORM_BUFFER
            , vk::ShaderStageFlags stageFlags = vk::ShaderStageFlags()
            );
        PassBufferInfo(const PassBufferInfo &);
        PassBufferInfo& operator=(const PassBufferInfo &);
    };

    struct PassBufferData {
        std::vector<PassBufferInfo::BufferInfo> buffers;
        uint32_t bindingPriority;
        BufferDescriptorType descriptorType;
        vk::ShaderStageFlags stageFlags;
        PassBufferData(std::vector<PassBufferInfo::BufferInfo> buffers = {}
            , uint32_t bindingPriority = 0u
            , BufferDescriptorType descriptorType = BufferDescriptorType::UNIFORM_BUFFER
            , vk::ShaderStageFlags stageFlags = vk::ShaderStageFlags()
            );
        PassBufferData(const PassBufferData &);
        PassBufferData(const PassBufferInfo &);
        PassBufferData& operator=(const PassBufferData &);
        PassBufferData& operator=(const PassBufferInfo &);
        PassBufferInfo getBufferInfo() const;
    };


    struct PassExtUniformBufferInfo 
    {
        UniformBufferData *pData;
        uint32_t subDataOffset;
        uint32_t subDataCount;
        uint32_t bindingPriority;
        PassExtUniformBufferInfo(UniformBufferData *pData = nullptr
            , uint32_t subDataOffset = 0u
            , uint32_t subDataCount = 0u
            , uint32_t bindingPriority = 0u
            );
        PassExtUniformBufferInfo(const PassExtUniformBufferInfo &);
        PassExtUniformBufferInfo& operator=(const PassExtUniformBufferInfo &);
    };


    struct PassData
    {    
        std::vector<std::string> arrDataNames;
        std::unordered_map<std::string, std::vector<Byte>> mapDatas;
        std::unordered_map<std::string, uint32_t> mapDataCounts;

        std::unordered_map<std::string, PassDataInfo> mapDataInfos;
        std::unordered_map<std::string, PassDataSizeInfo> mapDataSizeInfos;

        std::vector<std::string> arrBufferNames;
        std::unordered_map<std::string, PassBufferData> mapBuffers;

        std::vector<std::string> arrTexNames;
        std::unordered_map<std::string, PassTextureData> mapTextures;

        std::vector<std::string> arrExtUniformBufferNames;
        std::unordered_map<std::string, PassExtUniformBufferInfo> mapExtUniformBuffers;

        PassData();

        const std::vector<std::string> getArrBufferNames() const;
        Bool32 hasBuffer(std::string name) const;
        void addBuffer(std::string name, const PassBufferInfo &bufferInfo);
        void removeBuffer(std::string name);
        PassBufferInfo getBufferInfo(std::string name) const;
        const PassBufferData &getBufferData(std::string name) const;
        void setBuffer(std::string name, const PassBufferInfo &bufferInfo);

        const std::vector<std::string> getArrTextureNames() const;
        Bool32 hasTexture(std::string name) const;
        void addTexture(std::string name, const PassTextureInfo &texInfo);
        void removeTexture(std::string name);
        PassTextureInfo getTextureInfo(std::string name) const;
        const PassTextureData &getTextureData(std::string name) const;
        void setTexture(std::string name, const PassTextureInfo &texInfo);

        const std::vector<std::string> getExtUniformBufferNames() const;
        Bool32 hasExtUniformBuffer(std::string name) const;
        void addExtUniformBuffer(std::string name, const PassExtUniformBufferInfo &extUniformBufferInfo);
        void removeExtUniformBuffer(std::string name);
        const PassExtUniformBufferInfo &getExtUniformBuffer(std::string name) const;
        void setExtUniformBuffer(std::string name, const PassExtUniformBufferInfo &extUniformBufferInfo);

        const std::vector<std::string> getArrDataNames() const;
        Bool32 hasData(std::string name) const;
        void removeData(std::string name);
        const PassDataInfo &getDataInfo(std::string name) const;
        const PassDataSizeInfo &getDataSizeInfo(std::string name) const;

        void addData(const std::string name, const PassDataInfo &info, const PassDataSizeInfo &sizeInfo);
        void addData(const std::string name, const PassDataInfo &info, const void *src, uint32_t size);
        void getData(const std::string name, void *dst, uint32_t size, uint32_t offset) const;
        void setData(const std::string name, const PassDataInfo &info, const PassDataSizeInfo &sizeInfo);
        void setData(const std::string name, const void *src, uint32_t size, uint32_t offset);

        template<typename T>
        void addData(const std::string name, const PassDataInfo &info, const T &value);
        template<typename T>
        T getData(const std::string name) const;
        template<typename T>
        void setData(const std::string name, const T &value);
        
        template<typename T>
        void addData(const std::string name, const PassDataInfo &info, const std::vector<T> &values);
        template <typename T>
        std::vector<T> getData(const std::string name, const uint32_t count) const;
        template<typename T>
        void setData(const std::string name, const std::vector<T> &values);

        template<typename T>
        void addData(const std::string name, const PassDataInfo &info, const T * const pSrc, const uint32_t count);
        template<typename T>
        void getData(const std::string name, const T * const pDst, const uint32_t count);
        template<typename T>
        void setData(const std::string name, const T * const pSrc, const uint32_t count);

        uint32_t getDataBaseSize(const std::string name) const;

        uint32_t getDataSize(const std::string name) const;

        void memoryCopyData(const std::string name
            , void* dst
            , uint32_t offset
            , uint32_t elementStart
            , uint32_t maxElementCount) const;
    };
} //vg

#include "graphics/pass/pass_data.inl"

#endif //VG_PASS_DATA_HPP