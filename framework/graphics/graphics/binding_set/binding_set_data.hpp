#ifndef VG_BINDING_SET_DATA_HPP
#define VG_BINDING_SET_DATA_HPP

#include "graphics/global.hpp"
#include "graphics/buffer_data/buffer_data.hpp"
#include "graphics/texture/texture.hpp"

namespace vg {

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
    extern std::array<std::pair<BufferDescriptorType, vk::DescriptorType>, static_cast<size_t>(BufferDescriptorType::RANGE_SIZE)> arrBufferDescriptorTypeToVK;

    extern vk::DescriptorType tranImageDescriptorTypeToVK(ImageDescriptorType type);
    extern vk::DescriptorType tranBufferDescriptorTypeToVK(BufferDescriptorType type);
    
    struct BindingSetDataInfo {
        uint32_t layoutPriority;
        vk::ShaderStageFlags shaderStageFlags;
        BindingSetDataInfo(uint32_t layoutPriority = 0u
            , vk::ShaderStageFlags shaderStageFlags = vk::ShaderStageFlags()
        );
        BindingSetDataInfo(const BindingSetDataInfo &);
        BindingSetDataInfo& operator=(const BindingSetDataInfo &);
        
    };

    struct BindingSetDataSizeInfo {
        uint32_t size;

        BindingSetDataSizeInfo(uint32_t size = 0u);
        BindingSetDataSizeInfo(const BindingSetDataSizeInfo &);
        BindingSetDataSizeInfo& operator=(const BindingSetDataSizeInfo &);
        uint32_t getBufferSize() const;
    };

    struct BindingSetTextureInfo {
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
        BindingSetTextureInfo(SamplerTextureType textureType = SamplerTextureType::TEX_1D
            , uint32_t textureCount = 0u
            , const TextureInfo *pTextures = nullptr
            , uint32_t bindingPriority = 0u
            , ImageDescriptorType descriptorType = ImageDescriptorType::COMBINED_IMAGE_SAMPLER
            , vk::ShaderStageFlags stageFlags = vk::ShaderStageFlagBits::eFragment
            );
        BindingSetTextureInfo(const BindingSetTextureInfo &);
        BindingSetTextureInfo& operator=(const BindingSetTextureInfo &);
    };

    struct BindingSetTextureData {
        SamplerTextureType textureType;
        std::vector<BindingSetTextureInfo::TextureInfo> textures;
        uint32_t bindingPriority;
        ImageDescriptorType descriptorType;
        vk::ShaderStageFlags stageFlags;
        BindingSetTextureData(SamplerTextureType textureType = SamplerTextureType::TEX_1D
            , std::vector<BindingSetTextureInfo::TextureInfo> textures = {}
            , uint32_t bindingPriority = 0u
            , ImageDescriptorType descriptorType = ImageDescriptorType::COMBINED_IMAGE_SAMPLER
            , vk::ShaderStageFlags stageFlags = vk::ShaderStageFlagBits::eFragment
            );
        BindingSetTextureData(const BindingSetTextureData &);
        BindingSetTextureData(const BindingSetTextureInfo &);
        BindingSetTextureData& operator=(const BindingSetTextureData &);
        BindingSetTextureData& operator=(const BindingSetTextureInfo &);
        BindingSetTextureInfo getTextureInfo() const;
    };

    struct BindingSetBufferInfo {
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
        BindingSetBufferInfo(uint32_t bufferCount = 0u
            , const BufferInfo *pBuffers = nullptr
            , uint32_t bindingPriority = 0u
            , BufferDescriptorType descriptorType = BufferDescriptorType::UNIFORM_BUFFER
            , vk::ShaderStageFlags stageFlags = vk::ShaderStageFlags()
            );
        BindingSetBufferInfo(const BindingSetBufferInfo &);
        BindingSetBufferInfo& operator=(const BindingSetBufferInfo &);
    };

    struct BindingSetBufferData {
        std::vector<BindingSetBufferInfo::BufferInfo> buffers;
        uint32_t bindingPriority;
        BufferDescriptorType descriptorType;
        vk::ShaderStageFlags stageFlags;
        BindingSetBufferData(std::vector<BindingSetBufferInfo::BufferInfo> buffers = {}
            , uint32_t bindingPriority = 0u
            , BufferDescriptorType descriptorType = BufferDescriptorType::UNIFORM_BUFFER
            , vk::ShaderStageFlags stageFlags = vk::ShaderStageFlags()
            );
        BindingSetBufferData(const BindingSetBufferData &);
        BindingSetBufferData(const BindingSetBufferInfo &);
        BindingSetBufferData& operator=(const BindingSetBufferData &);
        BindingSetBufferData& operator=(const BindingSetBufferInfo &);
        BindingSetBufferInfo getBufferInfo() const;
    };

    struct BindingSetData
    {    
        std::vector<std::string> arrDataNames;
        std::unordered_map<std::string, std::vector<Byte>> mapDatas;
        std::unordered_map<std::string, uint32_t> mapDataCounts;

        std::unordered_map<std::string, BindingSetDataInfo> mapDataInfos;
        std::unordered_map<std::string, BindingSetDataSizeInfo> mapDataSizeInfos;

        std::vector<std::string> arrBufferNames;
        std::unordered_map<std::string, BindingSetBufferData> mapBuffers;

        std::vector<std::string> arrTexNames;
        std::unordered_map<std::string, BindingSetTextureData> mapTextures;

        BindingSetData();

        const std::vector<std::string> getArrDataNames() const;
        Bool32 hasData(std::string name) const;
        void removeData(std::string name);
        const BindingSetDataInfo &getDataInfo(std::string name) const;
        const BindingSetDataSizeInfo &getDataSizeInfo(std::string name) const;

        void addData(const std::string name, const BindingSetDataInfo &info, const BindingSetDataSizeInfo &sizeInfo);
        void addData(const std::string name, const BindingSetDataInfo &info, const void *src, uint32_t size);
        void getData(const std::string name, void *dst, uint32_t size, uint32_t offset) const;
        void setData(const std::string name, const BindingSetDataInfo &info, const BindingSetDataSizeInfo &sizeInfo);
        void setData(const std::string name, const void *src, uint32_t size, uint32_t offset);

        template<typename T>
        void addData(const std::string name, const BindingSetDataInfo &info, const T &value);
        template<typename T>
        T getData(const std::string name) const;
        template<typename T>
        void setData(const std::string name, const T &value);
        
        template<typename T>
        void addData(const std::string name, const BindingSetDataInfo &info, const std::vector<T> &values);
        template <typename T>
        std::vector<T> getData(const std::string name, const uint32_t count) const;
        template<typename T>
        void setData(const std::string name, const std::vector<T> &values);

        template<typename T>
        void addData(const std::string name, const BindingSetDataInfo &info, const T * const pSrc, const uint32_t count);
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
            , uint32_t maxElementCount
            ) const;


        const std::vector<std::string> getArrBufferNames() const;
        Bool32 hasBuffer(std::string name) const;
        void addBuffer(std::string name, const BindingSetBufferInfo &bufferInfo);
        void removeBuffer(std::string name);
        BindingSetBufferInfo getBufferInfo(std::string name) const;
        const BindingSetBufferData &getBufferData(std::string name) const;
        void setBuffer(std::string name, const BindingSetBufferInfo &bufferInfo);

        const std::vector<std::string> getArrTextureNames() const;
        Bool32 hasTexture(std::string name) const;
        void addTexture(std::string name, const BindingSetTextureInfo &texInfo);
        void removeTexture(std::string name);
        BindingSetTextureInfo getTextureInfo(std::string name) const;
        const BindingSetTextureData &getTextureData(std::string name) const;
        void setTexture(std::string name, const BindingSetTextureInfo &texInfo);
    };
} //vg

#include "graphics/binding_set/binding_set_data.inl"

#endif //VG_BINDING_SET_DATA_HPP