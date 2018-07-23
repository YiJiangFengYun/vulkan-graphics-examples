#include "graphics/pass/pass_data.hpp"

namespace vg
{
    PassDataInfo::PassDataInfo(uint32_t layoutPriority
        , vk::ShaderStageFlags shaderStageFlags
        )
        : layoutPriority(layoutPriority)
        , shaderStageFlags(shaderStageFlags)
    {
        
    }

    PassDataInfo::PassDataInfo(const PassDataInfo &target)
        : layoutPriority(target.layoutPriority)
        , shaderStageFlags(target.shaderStageFlags)
    {

    }

    PassDataInfo& PassDataInfo::operator=(const PassDataInfo &target)
    {
        layoutPriority = target.layoutPriority;
        shaderStageFlags = target.shaderStageFlags;
        return *this;
    }

    PassDataSizeInfo::PassDataSizeInfo(uint32_t size
        )
        : size(size)
    {
        
    }

    PassDataSizeInfo::PassDataSizeInfo(const PassDataSizeInfo &target)
        : size(target.size)
    {

    }

    PassDataSizeInfo& PassDataSizeInfo::operator=(const PassDataSizeInfo &target)
    {
        size = target.size;
        return *this;
    }

    uint32_t PassDataSizeInfo::getBufferSize() const
    {
        const auto &pPhysicalDevice = pApp->getPhysicalDevice();
        const auto &properties = pPhysicalDevice->getProperties();
        const auto &minOffsetAlignment = static_cast<float>(properties.limits.minUniformBufferOffsetAlignment);
        return static_cast<uint32_t>(std::ceil(size / minOffsetAlignment) * minOffsetAlignment);
    }

    PassTextureInfo::TextureInfo::TextureInfo(const Texture *pTexture
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

    PassTextureInfo::TextureInfo::TextureInfo(const TextureInfo &target)
        : pTexture(target.pTexture)
        , pImageView(target.pImageView)
        , pSampler(target.pSampler)
        , imageLayout(target.imageLayout)
    {

    }
            
    PassTextureInfo::TextureInfo& PassTextureInfo::TextureInfo::operator=(const TextureInfo &target)
    {
        pTexture = target.pTexture;
        pImageView = target.pImageView;
        pSampler = target.pSampler;
        imageLayout = target.imageLayout;
        return *this;
    }

    PassTextureData::PassTextureData(SamplerTextureType textureType
        , std::vector<PassTextureInfo::TextureInfo> textures
        , uint32_t bindingPriority
        , ImageDescriptorType descriptorType
        , vk::ShaderStageFlags stageFlags
        )
        : textureType(textureType)
        , textures(textures)
        , bindingPriority(bindingPriority)
        , descriptorType(descriptorType)
        , stageFlags(stageFlags)
    {

    }
        
    PassTextureData::PassTextureData(const PassTextureData &target)
        : textureType(target.textureType)
        , textures(target.textures)
        , bindingPriority(target.bindingPriority)
        , descriptorType(target.descriptorType)
        , stageFlags(target.stageFlags)
    {

    }
        
    PassTextureData::PassTextureData(const PassTextureInfo &target)
        : textureType(target.textureType)
        , textures()
        , bindingPriority(target.bindingPriority)
        , descriptorType(target.descriptorType)
        , stageFlags(target.stageFlags)
    {
        textures.resize(target.textureCount);
        memcpy(textures.data(), target.pTextures, 
            sizeof(PassTextureInfo::TextureInfo) * static_cast<size_t>(target.textureCount));
    }
        
    PassTextureData &PassTextureData::operator=(const PassTextureData &target)
    {
        textureType = target.textureType;
        textures = target.textures;
        bindingPriority = target.bindingPriority;
        descriptorType = target.descriptorType;
        stageFlags = target.stageFlags;
        return *this;
    }
        
    PassTextureData &PassTextureData::operator=(const PassTextureInfo &target)
    {
        textureType = target.textureType;
        textures.resize(target.textureCount);
        memcpy(textures.data(), target.pTextures, 
            sizeof(PassTextureInfo::TextureInfo) * static_cast<size_t>(target.textureCount));
        
        bindingPriority = target.bindingPriority;
        descriptorType = target.descriptorType;
        stageFlags = target.stageFlags;
        return *this;
    }
        
    PassTextureInfo PassTextureData::getTextureInfo() const
    {
        PassTextureInfo result = {
            textureType,
            static_cast<uint32_t>(textures.size()),
            textures.data(),
            bindingPriority,
            descriptorType,
            stageFlags,
        };
        return result;
    }

    PassTextureInfo::PassTextureInfo(SamplerTextureType textureType
        , uint32_t textureCount
        , const TextureInfo *pTextures
        , uint32_t bindingPriority
        , ImageDescriptorType descriptorType
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

    PassTextureInfo::PassTextureInfo(const PassTextureInfo &target)
        : textureType(target.textureType)
        , textureCount(target.textureCount)
        , pTextures(target.pTextures)
        , bindingPriority(target.bindingPriority)
        , descriptorType(target.descriptorType)
        , stageFlags(target.stageFlags)
    {

    }

    PassTextureInfo& PassTextureInfo::operator=(const PassTextureInfo &target)
    {
        textureType = target.textureType;
        textureCount = target.textureCount;
        pTextures = target.pTextures;
        bindingPriority = target.bindingPriority;
        descriptorType = target.descriptorType;
        stageFlags = target.stageFlags;
        return *this;
    }

    PassBufferInfo::BufferInfo::BufferInfo(const BufferData *pBuffer
        , uint32_t offset
        , uint32_t range
        )
        : pBuffer(pBuffer)
        , offset(offset)
        , range(range)
    {

    }
        
    PassBufferInfo::BufferInfo::BufferInfo(const BufferInfo &target)
        : pBuffer(target.pBuffer)
        , offset(target.offset)
        , range(target.range)
    {

    }
            
    PassBufferInfo::BufferInfo &PassBufferInfo::BufferInfo::operator=(const BufferInfo &target)
    {
        pBuffer = target.pBuffer;
        offset = target.offset;
        range = target.range;
        return *this;
    }


    PassBufferInfo::PassBufferInfo(uint32_t bufferCount
        , const BufferInfo *pBuffers
        , uint32_t bindingPriority
        , BufferDescriptorType descriptorType
        , vk::ShaderStageFlags stageFlags
        )
        : bufferCount(bufferCount)
        , pBuffers(pBuffers)
        , bindingPriority(bindingPriority)
        , descriptorType(descriptorType)
        , stageFlags(stageFlags)
    {
        
    }

    PassBufferInfo::PassBufferInfo(const PassBufferInfo &target)
        : bufferCount(target.bufferCount)
        , pBuffers(target.pBuffers)
        , bindingPriority(target.bindingPriority)
        , descriptorType(target.descriptorType)
        , stageFlags(target.stageFlags)
    {

    }

    PassBufferInfo& PassBufferInfo::operator=(const PassBufferInfo &target)
    {
        bufferCount = target.bufferCount;
        pBuffers = target.pBuffers;
        bindingPriority = target.bindingPriority;
        descriptorType = target.descriptorType;
        stageFlags = target.stageFlags;
        return *this;
    }

    PassBufferData::PassBufferData(std::vector<PassBufferInfo::BufferInfo> buffers
        , uint32_t bindingPriority
        , BufferDescriptorType descriptorType
        , vk::ShaderStageFlags stageFlags
        )
        : buffers(buffers)
        , bindingPriority(bindingPriority)
        , descriptorType(descriptorType)
        , stageFlags(stageFlags)
    {

    }
        
    PassBufferData::PassBufferData(const PassBufferData &target)
        : buffers(target.buffers)
        , bindingPriority(target.bindingPriority)
        , descriptorType(target.descriptorType)
        , stageFlags(target.stageFlags)
    {

    }
        
    PassBufferData::PassBufferData(const PassBufferInfo &target)
        : buffers()
        , bindingPriority(target.bindingPriority)
        , descriptorType(target.descriptorType)
        , stageFlags(target.stageFlags)
    {
        buffers.resize(target.bufferCount);
        memcpy(buffers.data(), target.pBuffers, 
            sizeof(PassBufferInfo::BufferInfo) * static_cast<size_t>(target.bufferCount));
    }
        
    PassBufferData &PassBufferData::operator=(const PassBufferData &target)
    {
        buffers = target.buffers;
        bindingPriority = target.bindingPriority;
        descriptorType = target.descriptorType;
        stageFlags = target.stageFlags;
        return *this;
    }
        
    PassBufferData &PassBufferData::operator=(const PassBufferInfo &target)
    {
        buffers.resize(target.bufferCount);
        memcpy(buffers.data(), target.pBuffers, 
            sizeof(PassBufferInfo::BufferInfo) * static_cast<size_t>(target.bufferCount));
        bindingPriority = target.bindingPriority;
        descriptorType = target.descriptorType;
        stageFlags = target.stageFlags;
        return *this;
    }
        
    PassBufferInfo PassBufferData::getBufferInfo() const
    {
        PassBufferInfo result = {
            static_cast<uint32_t>(buffers.size()),
            buffers.data(),
            bindingPriority,
            descriptorType,
            stageFlags,
        };
        return result;
    }

    PassExtUniformBufferInfo::PassExtUniformBufferInfo(UniformBufferData *pData
        , uint32_t subDataOffset
        , uint32_t subDataCount
        , uint32_t bindingPriority
        )
        : pData(pData)
        , subDataOffset(subDataOffset)
        , subDataCount(subDataCount)
        , bindingPriority(bindingPriority)
    {

    }
        
    PassExtUniformBufferInfo::PassExtUniformBufferInfo(const PassExtUniformBufferInfo &target)
        : pData(target.pData)
        , subDataOffset(target.subDataOffset)
        , subDataCount(target.subDataCount)
        , bindingPriority(target.bindingPriority)
    {

    }
        
    PassExtUniformBufferInfo& PassExtUniformBufferInfo::operator=(const PassExtUniformBufferInfo &target)
    {
        pData = target.pData;
        subDataOffset = target.subDataOffset;
        subDataCount = target.subDataCount;
        bindingPriority = target.bindingPriority;
        return *this;
    }

    PassData::PassData()
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

    const std::vector<std::string> PassData::getArrBufferNames() const
    {
        return arrBufferNames;
    }

    Bool32 PassData::hasBuffer(std::string name) const
    {
        return hasValue<PassBufferData>(name, mapBuffers);
    }

    void PassData::addBuffer(std::string name, const PassBufferInfo &bufferInfo)
    {
        PassBufferData bufferData(bufferInfo);
        addValue(name, bufferData, mapBuffers, arrBufferNames);
    }

    void PassData::removeBuffer(std::string name)
    {
        removeValue(name, mapBuffers, arrBufferNames);
    }

    PassBufferInfo PassData::getBufferInfo(std::string name) const
    {
        return getValue(name, mapBuffers, arrBufferNames).getBufferInfo();
    }

    const PassBufferData &PassData::getBufferData(std::string name) const
    {
        return getValue(name, mapBuffers, arrBufferNames);
    }

    void PassData::setBuffer(std::string name, const PassBufferInfo &bufferInfo)
    {
        getValue(name, mapBuffers, arrBufferNames) = bufferInfo;
    }

    const std::vector<std::string> PassData::getArrTextureNames() const
    {
        return arrTexNames;
    }

    Bool32 PassData::hasTexture(std::string name) const
    {
        return hasValue<PassTextureData>(name, mapTextures);
    }
        
    void PassData::addTexture(std::string name, const PassTextureInfo &texInfo)
    {
        PassTextureData textureData(texInfo);
        addValue(name, textureData, mapTextures, arrTexNames);
    }
        
    void PassData::removeTexture(std::string name)
    {
        removeValue(name, mapTextures, arrTexNames);
    }

    PassTextureInfo PassData::getTextureInfo(std::string name) const
    {
        return getValue(name, mapTextures, arrTexNames).getTextureInfo();
    }

    const PassTextureData &PassData::getTextureData(std::string name) const
    {
        return getValue(name, mapTextures, arrTexNames);
    }

    void PassData::setTexture(std::string name, const PassTextureInfo &texInfo)
    {
        getValue(name, mapTextures, arrTexNames) = texInfo;
    }

    const std::vector<std::string> PassData::getExtUniformBufferNames() const
    {
        return arrExtUniformBufferNames;
    }

    Bool32 PassData::hasExtUniformBuffer(std::string name) const
    {
        return hasValue(name, mapExtUniformBuffers, arrExtUniformBufferNames);
    }
        
    void PassData::addExtUniformBuffer(std::string name, const PassExtUniformBufferInfo &extUniformBufferInfo)
    {
        addValue(name, extUniformBufferInfo, mapExtUniformBuffers, arrExtUniformBufferNames);
    }
        
    void PassData::removeExtUniformBuffer(std::string name)
    {
        removeValue(name, mapExtUniformBuffers, arrExtUniformBufferNames);
    }
        
    const PassExtUniformBufferInfo &PassData::getExtUniformBuffer(std::string name) const
    {
        return getValue(name, mapExtUniformBuffers, arrExtUniformBufferNames);
    }

    void PassData::setExtUniformBuffer(std::string name, const PassExtUniformBufferInfo &extUniformBufferInfo)
    {
        setValue(name, extUniformBufferInfo, mapExtUniformBuffers, arrExtUniformBufferNames);
    }

    const std::vector<std::string> PassData::getArrDataNames() const
    {
        return arrDataNames;
    }

    Bool32 PassData::hasData(std::string name) const
    {
        return hasValue<std::vector<Byte>>(name, mapDatas);
    }
        
    void PassData::removeData(std::string name)
    {
        removeValue(name, mapDatas, arrDataNames);
        removeValue(name, mapDataCounts);
        removeValue(name, mapDataInfos);
        removeValue(name, mapDataSizeInfos);
    }

    const PassDataInfo &PassData::getDataInfo(std::string name) const
    {
        return getValue(name, mapDataInfos);
    }

    const PassDataSizeInfo &PassData::getDataSizeInfo(std::string name) const
    {
        return getValue(name, mapDataSizeInfos);
    }

    void PassData::addData(const std::string name, const PassDataInfo &info, const PassDataSizeInfo &sizeInfo)
    {
        std::vector<Byte> temp(sizeInfo.size);
        addValue(name, temp, mapDatas, arrDataNames);
        addValue(name, 1u, mapDataCounts);
        addValue(name, info, mapDataInfos);
        addValue(name, sizeInfo, mapDataSizeInfos);
    }

    void PassData::addData(const std::string name, const PassDataInfo &info, void *src, uint32_t size)
    {
        std::vector<Byte> temp(size);
        if (size != 0u && src != nullptr) memcpy(temp.data(), src, size);
        addValue(name, temp, mapDatas, arrDataNames);
        addValue(name, 1u, mapDataCounts);
        addValue(name, info, mapDataInfos);
        PassDataSizeInfo sizeInfo = {
            size
        };
        addValue(name, sizeInfo, mapDataSizeInfos);
    }

    void PassData::getData(const std::string name, void *dst, uint32_t size, uint32_t offset) const
    {
        const auto &bytes = getValue(name, mapDatas);        
        if (offset + size > static_cast<uint32_t>(bytes.size()))
            throw std::range_error("Out range of the saved material data!");
        memcpy(dst, (char *)(bytes.data()) + offset, size);
    }

    void PassData::setData(const std::string name, const PassDataInfo &info, const PassDataSizeInfo &sizeInfo)
    {
        setValue(name, info, mapDataInfos);
        setValue(name, sizeInfo, mapDataSizeInfos);
    }

    void PassData::setData(const std::string name, void *src, uint32_t size, uint32_t offset)
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
                PassDataSizeInfo sizeInfo = {
                    newSize
                };
                setValue(name, sizeInfo, mapDataSizeInfos);
            }
            if(src) memcpy((char *)(mapDatas[name].data()) + offset, src, size);
            setValue(name, 1u, mapDataCounts);
        }
    }

    uint32_t PassData::getDataBaseSize(const std::string name) const
    {
        const auto& bytes = getValue(name, mapDatas);
        const auto& count = getValue(name, mapDataCounts);
        return static_cast<uint32_t>(bytes.size()) / count;
    }

    uint32_t PassData::getDataSize(const std::string name) const
    {
        const auto& bytes = getValue(name, mapDatas);
        return static_cast<uint32_t>(bytes.size());
    }

    void PassData::memoryCopyData(const std::string name
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
} //vg