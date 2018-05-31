#include "graphics/pass/pass_data.hpp"

namespace vg
{
    PassDataInfo::PassDataInfo(vk::ShaderStageFlags shaderStageFlags
        )
        : shaderStageFlags(shaderStageFlags)
    {
        
    }

    PassDataInfo::PassDataInfo(const PassDataInfo &target)
        : shaderStageFlags(target.shaderStageFlags)
    {

    }

    PassDataInfo& PassDataInfo::operator=(const PassDataInfo &target)
    {
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

    PassTextureInfo::PassTextureInfo(const Texture *pTexture
        , const Texture::ImageView *pImageView
        , const Texture::Sampler *pSampler
        , vk::ImageLayout imageLayout
        , ImageDescriptorType descriptorType
        , vk::ShaderStageFlags stageFlags
    )
        : pTexture(pTexture)
        , pImageView(pImageView)
        , pSampler(pSampler)
        , imageLayout(imageLayout)
        , descriptorType(descriptorType)
        , stageFlags(stageFlags)
    {

    }

    PassTextureInfo::PassTextureInfo(const PassTextureInfo &target)
        : pTexture(target.pTexture)
        , pImageView(target.pImageView)
        , pSampler(target.pSampler)
        , imageLayout(target.imageLayout)
        , descriptorType(target.descriptorType)
        , stageFlags(target.stageFlags)
    {

    }

    PassTextureInfo& PassTextureInfo::operator=(const PassTextureInfo &target)
    {
        pTexture = target.pTexture;
        pImageView = target.pImageView;
        pSampler = target.pSampler;
        imageLayout = target.imageLayout;
        descriptorType = target.descriptorType;
        stageFlags = target.stageFlags;
        return *this;
    }


    PassBufferInfo::PassBufferInfo(const BufferData *pBuffer
        , uint32_t offset
        , uint32_t range
        , BufferDescriptorType descriptorType
        , vk::ShaderStageFlags stageFlags
        )
        : pBuffer(pBuffer)
        , offset(offset)
        , range(range)
        , descriptorType(descriptorType)
        , stageFlags(stageFlags)
    {
        
    }

    PassBufferInfo::PassBufferInfo(const PassBufferInfo &target)
        : pBuffer(target.pBuffer)
        , offset(target.offset)
        , range(target.range)
        , descriptorType(target.descriptorType)
        , stageFlags(target.stageFlags)
    {

    }

    PassBufferInfo& PassBufferInfo::operator=(const PassBufferInfo &target)
    {
        pBuffer = target.pBuffer;
        offset = target.offset;
        range = target.range;
        descriptorType = target.descriptorType;
        stageFlags = target.stageFlags;
        return *this;
    }

    PassData::PassData(PassDataInfo info
        , PassDataSizeInfo sizeInfo
        , std::vector<Byte> data
        , uint32_t count
        )
        : info(info)
        , sizeInfo(sizeInfo)
        , data(data)
        , count(count)
    {

    }

    PassDatas::PassDatas()
       : datas()
       , buffers()
       , textures()
    {

    }

    const std::set<SlotType> &PassDatas::getBufferSlots() const
    {
        return buffers.getSlots();
    }

    Bool32 PassDatas::hasBuffer(const SlotType &slot) const
    {
        return buffers.hasValue(slot);
    }

    void PassDatas::addBuffer(const SlotType &slot, const PassBufferInfo &bufferInfo)
    {
        buffers.addValue(slot, bufferInfo);
    }

    void PassDatas::removeBuffer(const SlotType &slot)
    {
        buffers.removeValue(slot);
    }

    const PassBufferInfo &PassDatas::getBuffer(const SlotType &slot) const
    {
        return buffers.getValue(slot);
    }

    void PassDatas::setBuffer(const SlotType &slot, const PassBufferInfo &bufferInfo)
    {
        buffers.setValue(slot, bufferInfo);
    }

    const std::set<SlotType> &PassDatas::getTextureSlots() const
    {
        return textures.getSlots();
    }

    Bool32 PassDatas::hasTexture(const SlotType &slot) const
    {
        return textures.hasValue(slot);
    }
        
    void PassDatas::addTexture(const SlotType &slot, const PassTextureInfo &texInfo)
    {
        textures.addValue(slot, texInfo);
    }
        
    void PassDatas::removeTexture(const SlotType &slot)
    {
        textures.removeValue(slot);
    }

    const PassTextureInfo &PassDatas::getTexture(const SlotType &slot) const
    {
        return textures.getValue(slot);
    }

    void PassDatas::setTexture(const SlotType &slot, const PassTextureInfo &texInfo)
    {
        textures.setValue(slot, texInfo);
    }

    const std::set<SlotType> &PassDatas::getDataSlots() const
    {
        return datas.getSlots();
    }

    Bool32 PassDatas::hasData(const SlotType &slot) const
    {
        return datas.hasValue(slot);
    }
        
    void PassDatas::removeData(const SlotType &slot)
    {
        datas.removeValue(slot);
    }

    const PassDataInfo &PassDatas::getDataInfo(const SlotType &slot) const
    {
        return datas.getValue(slot).info;
    }

    const PassDataSizeInfo &PassDatas::getDataSizeInfo(const SlotType &slot) const
    {
        return datas.getValue(slot).sizeInfo;
    }

    void PassDatas::addData(const SlotType &slot, const PassDataInfo &info, const PassDataSizeInfo &sizeInfo)
    {
        std::vector<Byte> temp(sizeInfo.size);
        PassData tempData(info, sizeInfo, temp, 1u);
        datas.addValue(slot, std::move(tempData));
    }

    void PassDatas::addData(const SlotType &slot, const PassDataInfo &info, void *src, uint32_t size)
    {

        std::vector<Byte> temp(size);
        if (size != 0u && src != nullptr) memcpy(temp.data(), src, size);
        PassDataSizeInfo sizeInfo = {
            size
        };
        PassData tempData(info, sizeInfo, temp, 1u);
        datas.addValue(slot, std::move(tempData));
    }

    void PassDatas::getData(const SlotType &slot, void *dst, uint32_t size, uint32_t offset) const
    {
        const auto &bytes = datas.getValue(slot).data;
        if (offset + size > static_cast<uint32_t>(bytes.size()))
            throw std::range_error("Out range of the saved material data!");
        memcpy(dst, (char *)(bytes.data()) + offset, size);
    }

    void PassDatas::setData(const SlotType &slot, const PassDataInfo &info, const PassDataSizeInfo &sizeInfo)
    {
        auto &value = datas.getValue(slot);
        value.info = info;
        value.sizeInfo = sizeInfo;
    }

    void PassDatas::setData(const SlotType &slot, void *src, uint32_t size, uint32_t offset)
    {
        auto &value = datas.getValue(slot);
        auto &bytes = value.data;
        uint32_t newSize = offset + size;
        if (newSize > static_cast<uint32_t>(bytes.size()))
        {
            bytes.resize(newSize);
            value.sizeInfo.size = newSize;
        }
        if(src) memcpy(bytes.data() + offset, src, size);
        value.count = 1u;
    }

    uint32_t PassDatas::getDataBaseSize(const SlotType &slot) const
    {
        const auto& bytes = datas.getValue(slot).data;
        const auto& count = datas.getValue(slot).count;
        return static_cast<uint32_t>(bytes.size()) / count;
    }

    uint32_t PassDatas::getDataSize(const SlotType &slot) const
    {
        const auto& bytes = datas.getValue(slot).data;
        return static_cast<uint32_t>(bytes.size());
    }

    void PassDatas::memoryCopyData(const SlotType &slot
        , void* dst
        , uint32_t offset
        , uint32_t elementStart
        , uint32_t maxElementCount) const
    {
        auto &value = datas.getValue(slot);
        const auto& bytes = value.data;
        const auto& count = value.count;
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