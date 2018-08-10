#include "graphics/binding_set/binding_set_data.hpp"

namespace vg
{
///Data Info

    BindingSetDataInfo::BindingSetDataInfo(uint32_t layoutPriority
        , vk::ShaderStageFlags shaderStageFlags
        )
        : layoutPriority(layoutPriority)
        , shaderStageFlags(shaderStageFlags)
    {
        
    }

    BindingSetDataInfo::BindingSetDataInfo(const BindingSetDataInfo &target)
        : layoutPriority(target.layoutPriority)
        , shaderStageFlags(target.shaderStageFlags)
    {

    }

    BindingSetDataInfo& BindingSetDataInfo::operator=(const BindingSetDataInfo &target)
    {
        layoutPriority = target.layoutPriority;
        shaderStageFlags = target.shaderStageFlags;
        return *this;
    }

    BindingSetDataSizeInfo::BindingSetDataSizeInfo(uint32_t size
        )
        : size(size)
    {
        
    }

    BindingSetDataSizeInfo::BindingSetDataSizeInfo(const BindingSetDataSizeInfo &target)
        : size(target.size)
    {

    }

    BindingSetDataSizeInfo& BindingSetDataSizeInfo::operator=(const BindingSetDataSizeInfo &target)
    {
        size = target.size;
        return *this;
    }

    uint32_t BindingSetDataSizeInfo::getBufferSize() const
    {
        const auto &pPhysicalDevice = pApp->getPhysicalDevice();
        const auto &properties = pPhysicalDevice->getProperties();
        const auto &minOffsetAlignment = static_cast<float>(properties.limits.minUniformBufferOffsetAlignment);
        return static_cast<uint32_t>(std::ceil(size / minOffsetAlignment) * minOffsetAlignment);
    }

/// Texture Info
    BindingSetTextureInfo::TextureInfo::TextureInfo(const Texture *pTexture
        , const Texture::ImageView *pImageView
        , const Texture::Sampler *pSampler
        , vk::ImageLayout imageLayout
        )
        : pTexture(pTexture)
        , pImageView(pImageView)
        , pSampler(pSampler)
        , imageLayout(imageLayout)
    {

    }

    BindingSetTextureInfo::TextureInfo::TextureInfo(const TextureInfo &target)
        : pTexture(target.pTexture)
        , pImageView(target.pImageView)
        , pSampler(target.pSampler)
        , imageLayout(target.imageLayout)
    {

    }
            
    BindingSetTextureInfo::TextureInfo& BindingSetTextureInfo::TextureInfo::operator=(const TextureInfo &target)
    {
        pTexture = target.pTexture;
        pImageView = target.pImageView;
        pSampler = target.pSampler;
        imageLayout = target.imageLayout;
        return *this;
    }

    BindingSetTextureData::BindingSetTextureData(SamplerTextureType textureType
        , std::vector<BindingSetTextureInfo::TextureInfo> textures
        , uint32_t bindingPriority
        , vk::DescriptorType descriptorType
        , vk::ShaderStageFlags stageFlags
        )
        : textureType(textureType)
        , textures(textures)
        , bindingPriority(bindingPriority)
        , descriptorType(descriptorType)
        , stageFlags(stageFlags)
    {

    }
        
    BindingSetTextureData::BindingSetTextureData(const BindingSetTextureData &target)
        : textureType(target.textureType)
        , textures(target.textures)
        , bindingPriority(target.bindingPriority)
        , descriptorType(target.descriptorType)
        , stageFlags(target.stageFlags)
    {

    }
        
    BindingSetTextureData::BindingSetTextureData(const BindingSetTextureInfo &target)
        : textureType(target.textureType)
        , textures()
        , bindingPriority(target.bindingPriority)
        , descriptorType(target.descriptorType)
        , stageFlags(target.stageFlags)
    {
        textures.resize(target.textureCount);
        memcpy(textures.data(), target.pTextures, 
            sizeof(BindingSetTextureInfo::TextureInfo) * static_cast<size_t>(target.textureCount));
    }
        
    BindingSetTextureData &BindingSetTextureData::operator=(const BindingSetTextureData &target)
    {
        textureType = target.textureType;
        textures = target.textures;
        bindingPriority = target.bindingPriority;
        descriptorType = target.descriptorType;
        stageFlags = target.stageFlags;
        return *this;
    }
        
    BindingSetTextureData &BindingSetTextureData::operator=(const BindingSetTextureInfo &target)
    {
        textureType = target.textureType;
        textures.resize(target.textureCount);
        memcpy(textures.data(), target.pTextures, 
            sizeof(BindingSetTextureInfo::TextureInfo) * static_cast<size_t>(target.textureCount));
        
        bindingPriority = target.bindingPriority;
        descriptorType = target.descriptorType;
        stageFlags = target.stageFlags;
        return *this;
    }
        
    BindingSetTextureInfo BindingSetTextureData::getTextureInfo() const
    {
        BindingSetTextureInfo result = {
            textureType,
            static_cast<uint32_t>(textures.size()),
            textures.data(),
            bindingPriority,
            descriptorType,
            stageFlags,
        };
        return result;
    }

    BindingSetTextureInfo::BindingSetTextureInfo(SamplerTextureType textureType
        , uint32_t textureCount
        , const TextureInfo *pTextures
        , uint32_t bindingPriority
        , vk::DescriptorType descriptorType
        , vk::ShaderStageFlags stageFlags
        )
        : textureType(textureType)
        , textureCount(textureCount)
        , pTextures(pTextures)
        , bindingPriority(bindingPriority)
        , descriptorType(descriptorType)
        , stageFlags(stageFlags)
    {

    }

    BindingSetTextureInfo::BindingSetTextureInfo(const BindingSetTextureInfo &target)
        : textureType(target.textureType)
        , textureCount(target.textureCount)
        , pTextures(target.pTextures)
        , bindingPriority(target.bindingPriority)
        , descriptorType(target.descriptorType)
        , stageFlags(target.stageFlags)
    {

    }

    BindingSetTextureInfo& BindingSetTextureInfo::operator=(const BindingSetTextureInfo &target)
    {
        textureType = target.textureType;
        textureCount = target.textureCount;
        pTextures = target.pTextures;
        bindingPriority = target.bindingPriority;
        descriptorType = target.descriptorType;
        stageFlags = target.stageFlags;
        return *this;
    }


///Buffer Info
    BindingSetBufferInfo::BufferInfo::BufferInfo(const BufferData *pBuffer
        , uint32_t offset
        , uint32_t range
        )
        : pBuffer(pBuffer)
        , offset(offset)
        , range(range)
    {

    }
        
    BindingSetBufferInfo::BufferInfo::BufferInfo(const BufferInfo &target)
        : pBuffer(target.pBuffer)
        , offset(target.offset)
        , range(target.range)
    {

    }
            
    BindingSetBufferInfo::BufferInfo &BindingSetBufferInfo::BufferInfo::operator=(const BufferInfo &target)
    {
        pBuffer = target.pBuffer;
        offset = target.offset;
        range = target.range;
        return *this;
    }


    BindingSetBufferInfo::BindingSetBufferInfo(uint32_t bufferCount
        , const BufferInfo *pBuffers
        , uint32_t bindingPriority
        , vk::DescriptorType descriptorType
        , vk::ShaderStageFlags stageFlags
        )
        : bufferCount(bufferCount)
        , pBuffers(pBuffers)
        , bindingPriority(bindingPriority)
        , descriptorType(descriptorType)
        , stageFlags(stageFlags)
    {
        
    }

    BindingSetBufferInfo::BindingSetBufferInfo(const BindingSetBufferInfo &target)
        : bufferCount(target.bufferCount)
        , pBuffers(target.pBuffers)
        , bindingPriority(target.bindingPriority)
        , descriptorType(target.descriptorType)
        , stageFlags(target.stageFlags)
    {

    }

    BindingSetBufferInfo& BindingSetBufferInfo::operator=(const BindingSetBufferInfo &target)
    {
        bufferCount = target.bufferCount;
        pBuffers = target.pBuffers;
        bindingPriority = target.bindingPriority;
        descriptorType = target.descriptorType;
        stageFlags = target.stageFlags;
        return *this;
    }

    BindingSetBufferData::BindingSetBufferData(std::vector<BindingSetBufferInfo::BufferInfo> buffers
        , uint32_t bindingPriority
        , vk::DescriptorType descriptorType
        , vk::ShaderStageFlags stageFlags
        )
        : buffers(buffers)
        , bindingPriority(bindingPriority)
        , descriptorType(descriptorType)
        , stageFlags(stageFlags)
    {

    }
        
    BindingSetBufferData::BindingSetBufferData(const BindingSetBufferData &target)
        : buffers(target.buffers)
        , bindingPriority(target.bindingPriority)
        , descriptorType(target.descriptorType)
        , stageFlags(target.stageFlags)
    {

    }
        
    BindingSetBufferData::BindingSetBufferData(const BindingSetBufferInfo &target)
        : buffers()
        , bindingPriority(target.bindingPriority)
        , descriptorType(target.descriptorType)
        , stageFlags(target.stageFlags)
    {
        buffers.resize(target.bufferCount);
        memcpy(buffers.data(), target.pBuffers, 
            sizeof(BindingSetBufferInfo::BufferInfo) * static_cast<size_t>(target.bufferCount));
    }
        
    BindingSetBufferData &BindingSetBufferData::operator=(const BindingSetBufferData &target)
    {
        buffers = target.buffers;
        bindingPriority = target.bindingPriority;
        descriptorType = target.descriptorType;
        stageFlags = target.stageFlags;
        return *this;
    }
        
    BindingSetBufferData &BindingSetBufferData::operator=(const BindingSetBufferInfo &target)
    {
        buffers.resize(target.bufferCount);
        memcpy(buffers.data(), target.pBuffers, 
            sizeof(BindingSetBufferInfo::BufferInfo) * static_cast<size_t>(target.bufferCount));
        bindingPriority = target.bindingPriority;
        descriptorType = target.descriptorType;
        stageFlags = target.stageFlags;
        return *this;
    }
        
    BindingSetBufferInfo BindingSetBufferData::getBufferInfo() const
    {
        BindingSetBufferInfo result = {
            static_cast<uint32_t>(buffers.size()),
            buffers.data(),
            bindingPriority,
            descriptorType,
            stageFlags,
        };
        return result;
    }


///Data
    BindingSetData::BindingSetData()
       : arrDataNames()
       , mapDatas()
       , mapDataCounts()
       , mapDataInfos()
       , mapDataSizeInfos()
       , arrBufferNames()
       , mapBuffers()
       , arrTexNames()
       , mapTextures()
    {

    }

        const std::vector<std::string> BindingSetData::getArrDataNames() const
    {
        return arrDataNames;
    }

    Bool32 BindingSetData::hasData(std::string name) const
    {
        return hasValue<std::vector<Byte>>(name, mapDatas);
    }
        
    void BindingSetData::removeData(std::string name)
    {
        removeValue(name, mapDatas, arrDataNames);
        removeValue(name, mapDataCounts);
        removeValue(name, mapDataInfos);
        removeValue(name, mapDataSizeInfos);
    }

    const BindingSetDataInfo &BindingSetData::getDataInfo(std::string name) const
    {
        return getValue(name, mapDataInfos);
    }

    const BindingSetDataSizeInfo &BindingSetData::getDataSizeInfo(std::string name) const
    {
        return getValue(name, mapDataSizeInfos);
    }

    void BindingSetData::addData(const std::string name, const BindingSetDataInfo &info, const BindingSetDataSizeInfo &sizeInfo)
    {
        std::vector<Byte> temp(sizeInfo.size);
        addValue(name, temp, mapDatas, arrDataNames);
        addValue(name, 1u, mapDataCounts);
        addValue(name, info, mapDataInfos);
        addValue(name, sizeInfo, mapDataSizeInfos);
    }

    void BindingSetData::addData(const std::string name, const BindingSetDataInfo &info, const void *src, uint32_t size)
    {
        std::vector<Byte> temp(size);
        if (size != 0u && src != nullptr) memcpy(temp.data(), src, size);
        addValue(name, temp, mapDatas, arrDataNames);
        addValue(name, 1u, mapDataCounts);
        addValue(name, info, mapDataInfos);
        BindingSetDataSizeInfo sizeInfo = {
            size
        };
        addValue(name, sizeInfo, mapDataSizeInfos);
    }

    void BindingSetData::getData(const std::string name, void *dst, uint32_t size, uint32_t offset) const
    {
        const auto &bytes = getValue(name, mapDatas);        
        if (offset + size > static_cast<uint32_t>(bytes.size()))
            throw std::range_error("Out range of the saved material data!");
        memcpy(dst, (char *)(bytes.data()) + offset, size);
    }

    void BindingSetData::setData(const std::string name, const BindingSetDataInfo &info, const BindingSetDataSizeInfo &sizeInfo)
    {
        setValue(name, info, mapDataInfos);
        setValue(name, sizeInfo, mapDataSizeInfos);
    }

    void BindingSetData::setData(const std::string name, const void *src, uint32_t size, uint32_t offset)
    {
        const auto& iterator = mapDatas.find(name);
        if (iterator == mapDatas.cend())
        {
            throw std::runtime_error("Map don't has item whose key: " + name);
        }
        else
        {
            if (offset + size > static_cast<uint32_t>(mapDatas[name].size()))
            {
                uint32_t newSize = offset + size;
                mapDatas[name].resize(newSize);
                BindingSetDataSizeInfo sizeInfo = {
                    newSize
                };
                setValue(name, sizeInfo, mapDataSizeInfos);
            }
            if(src) memcpy((char *)(mapDatas[name].data()) + offset, src, size);
            setValue(name, 1u, mapDataCounts);
        }
    }

    uint32_t BindingSetData::getDataBaseSize(const std::string name) const
    {
        const auto& bytes = getValue(name, mapDatas);
        const auto& count = getValue(name, mapDataCounts);
        return static_cast<uint32_t>(bytes.size()) / count;
    }

    uint32_t BindingSetData::getDataSize(const std::string name) const
    {
        const auto& bytes = getValue(name, mapDatas);
        return static_cast<uint32_t>(bytes.size());
    }

    void BindingSetData::memoryCopyData(const std::string name
        , void* dst
        , uint32_t offset
        , uint32_t elementStart
        , uint32_t maxElementCount) const
    {
        const auto& bytes = getValue(name, mapDatas);
        const auto& count = getValue(name, mapDataCounts);
        uint32_t baseSize = static_cast<uint32_t>(bytes.size()) / count;
        char *ptr = static_cast<char *>(dst);
        ptr += offset;
        uint32_t finalElementCount = std::max(0u, std::min(count - elementStart, maxElementCount));
        if (finalElementCount == 0) return;
        uint32_t srcOffset = elementStart * baseSize;
        uint32_t srcSize = finalElementCount * baseSize;
        std::memcpy(ptr, bytes.data() + srcOffset, srcSize);
    }

    const std::vector<std::string> BindingSetData::getArrBufferNames() const
    {
        return arrBufferNames;
    }

    Bool32 BindingSetData::hasBuffer(std::string name) const
    {
        return hasValue<BindingSetBufferData>(name, mapBuffers);
    }

    void BindingSetData::addBuffer(std::string name, const BindingSetBufferInfo &bufferInfo)
    {
        BindingSetBufferData bufferData(bufferInfo);
        addValue(name, bufferData, mapBuffers, arrBufferNames);
    }

    void BindingSetData::removeBuffer(std::string name)
    {
        removeValue(name, mapBuffers, arrBufferNames);
    }

    BindingSetBufferInfo BindingSetData::getBufferInfo(std::string name) const
    {
        return getValue(name, mapBuffers, arrBufferNames).getBufferInfo();
    }

    const BindingSetBufferData &BindingSetData::getBufferData(std::string name) const
    {
        return getValue(name, mapBuffers, arrBufferNames);
    }

    void BindingSetData::setBuffer(std::string name, const BindingSetBufferInfo &bufferInfo)
    {
        getValue(name, mapBuffers, arrBufferNames) = bufferInfo;
    }

    const std::vector<std::string> BindingSetData::getArrTextureNames() const
    {
        return arrTexNames;
    }

    Bool32 BindingSetData::hasTexture(std::string name) const
    {
        return hasValue<BindingSetTextureData>(name, mapTextures);
    }
        
    void BindingSetData::addTexture(std::string name, const BindingSetTextureInfo &texInfo)
    {
        BindingSetTextureData textureData(texInfo);
        addValue(name, textureData, mapTextures, arrTexNames);
    }
        
    void BindingSetData::removeTexture(std::string name)
    {
        removeValue(name, mapTextures, arrTexNames);
    }

    BindingSetTextureInfo BindingSetData::getTextureInfo(std::string name) const
    {
        return getValue(name, mapTextures, arrTexNames).getTextureInfo();
    }

    const BindingSetTextureData &BindingSetData::getTextureData(std::string name) const
    {
        return getValue(name, mapTextures, arrTexNames);
    }

    void BindingSetData::setTexture(std::string name, const BindingSetTextureInfo &texInfo)
    {
        getValue(name, mapTextures, arrTexNames) = texInfo;
    }
} //vg