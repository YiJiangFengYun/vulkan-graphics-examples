#ifndef VG_PASS_DATA_HPP
#define VG_PASS_DATA_HPP

#include "graphics/global.hpp"
#include "graphics/buffer_data/buffer_data.hpp"
#include "graphics/texture/texture.hpp"
#include "graphics/pass/pass_option.hpp"
#include "graphics/util/slot_map.hpp"

#define VG_PASS_BUILDIN_DATA_LAYOUT_SLOT 0
#define VG_PASS_OTHER_DATA_MIN_LAYOUT_SLOT 1

#define VG_PASS_PRE_Z_TEXTURE_BINDING_SLOT 0
#define VG_PASS_POST_RENDER_TEXTURE_BINDING_SLOT 1
#define VG_PASS_OTHER_MIN_BINDING_SLOT 2

namespace vg
{
    struct PassDataInfo {
        vk::ShaderStageFlags shaderStageFlags;
        PassDataInfo(vk::ShaderStageFlags shaderStageFlags = vk::ShaderStageFlags()
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
        const Texture *pTexture;
        const Texture::ImageView *pImageView;
        const Texture::Sampler *pSampler;
        vk::ImageLayout imageLayout;            
        ImageDescriptorType descriptorType;
        vk::ShaderStageFlags stageFlags;
        PassTextureInfo(const Texture *pTexture = nullptr
            , const Texture::ImageView *pImageView = nullptr
            , const Texture::Sampler *pSampler = nullptr
            , vk::ImageLayout imageLayout = vk::ImageLayout::eUndefined
            , ImageDescriptorType descriptorType = ImageDescriptorType::COMBINED_IMAGE_SAMPLER
            , vk::ShaderStageFlags stageFlags = vk::ShaderStageFlagBits::eFragment
            );
        PassTextureInfo(const PassTextureInfo &);
        PassTextureInfo& operator=(const PassTextureInfo &);
    };

    struct PassBufferInfo {
        const BufferData *pBuffer;
        uint32_t offset;
        uint32_t range;
        BufferDescriptorType descriptorType;
        vk::ShaderStageFlags stageFlags;
        PassBufferInfo(const BufferData *pBuffer = nullptr
            , uint32_t offset = 0u
            , uint32_t range = 0u
            , BufferDescriptorType descriptorType = BufferDescriptorType::UNIFORM_BUFFER
            , vk::ShaderStageFlags stageFlags = vk::ShaderStageFlags()
            );
        PassBufferInfo(const PassBufferInfo &);
        PassBufferInfo& operator=(const PassBufferInfo &);
    };

    struct PassData
    {
        PassDataInfo info;
        PassDataSizeInfo sizeInfo;
        std::vector<Byte> data;
        uint32_t count;

        PassData(PassDataInfo info = PassDataInfo()
            , PassDataSizeInfo sizeInfo = PassDataSizeInfo()
            , std::vector<Byte> data = std::vector<Byte>()
            , uint32_t count = 0u
            );
    };


    struct PassDatas
    {
        using DataSlotMapType = SlotMap<PassData>;
        using BufferSlotMapType = SlotMap<PassBufferInfo>;
        using TextureSlotMapType = SlotMap<PassTextureInfo>;

        DataSlotMapType datas;
        BufferSlotMapType buffers;
        TextureSlotMapType textures;
    
        PassDatas();

        const std::set<SlotType> &getBufferSlots() const;
        Bool32 hasBuffer(const SlotType &slot) const;
        void addBuffer(const SlotType &slot, const PassBufferInfo &bufferInfo);
        void removeBuffer(const SlotType &slot);
        const PassBufferInfo &getBuffer(const SlotType &slot) const;
        void setBuffer(const SlotType &slot, const PassBufferInfo &bufferInfo);

        const std::set<SlotType> &getTextureSlots() const;
        Bool32 hasTexture(const SlotType &slot) const;
        void addTexture(const SlotType &slot, const PassTextureInfo &texInfo);
        void removeTexture(const SlotType &slot);
        const PassTextureInfo &getTexture(const SlotType &slot) const;
        void setTexture(const SlotType &slot, const PassTextureInfo &texInfo);

        const std::set<SlotType> &getDataSlots() const;
        Bool32 hasData(const SlotType &slot) const;
        void removeData(const SlotType &slot);
        const PassDataInfo &getDataInfo(const SlotType &slot) const;
        const PassDataSizeInfo &getDataSizeInfo(const SlotType &slot) const;

        void addData(const SlotType &slot, const PassDataInfo &info, const PassDataSizeInfo &sizeInfo);
        void addData(const SlotType &slot, const PassDataInfo &info, void *src, uint32_t size);
        void getData(const SlotType &slot, void *dst, uint32_t size, uint32_t offset) const;
        void setData(const SlotType &slot, const PassDataInfo &info, const PassDataSizeInfo &sizeInfo);
        void setData(const SlotType &slot, void *src, uint32_t size, uint32_t offset);

        template<typename T>
        void addData(const SlotType &slot, const PassDataInfo &info, const T &value);
        template<typename T>
        T getData(const SlotType &slot) const;
        template<typename T>
        void setData(const SlotType &slot, const T &value);
        
        template<typename T>
        void addData(const SlotType &slot, const PassDataInfo &info, const std::vector<T> &values);
        template <typename T>
        std::vector<T> getData(const SlotType &slot, const uint32_t count) const;
        template<typename T>
        void setData(const SlotType &slot, const std::vector<T> &values);

        template<typename T>
        void addData(const SlotType &slot, const PassDataInfo &info, const T * const pSrc, const uint32_t count);
        template<typename T>
        void getData(const SlotType &slot, const T * const pDst, const uint32_t count);
        template<typename T>
        void setData(const SlotType &slot, const T * const pSrc, const uint32_t count);

        uint32_t getDataBaseSize(const SlotType &slot) const;

        uint32_t getDataSize(const SlotType &slot) const;

        void memoryCopyData(const SlotType &slot
            , void* dst
            , uint32_t offset
            , uint32_t elementStart
            , uint32_t maxElementCount) const;
    };
} //vg

#include "graphics/pass/pass_data.inl"

#endif //VG_PASS_DATA_HPP