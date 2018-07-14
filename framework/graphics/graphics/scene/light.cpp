#include "graphics/scene/light.hpp"

namespace vg
{
    LightDataInfo::LightDataInfo(uint32_t layoutPriority)
        : layoutPriority(layoutPriority)
    {

    }

    LightDataInfo::LightDataInfo(const LightDataInfo &target)
        : layoutPriority(target.layoutPriority)
    {

    }
        
    LightDataInfo &LightDataInfo::operator=(const LightDataInfo &target)
    {
        layoutPriority = target.layoutPriority;
        return *this;
    }

    LightDataSizeInfo::LightDataSizeInfo(uint32_t size)
        : size(size)
    {

    }

    LightDataSizeInfo::LightDataSizeInfo(const LightDataSizeInfo &target)
        : size(target.size)
    {

    }
        
    LightDataSizeInfo &LightDataSizeInfo::operator=(const LightDataSizeInfo &target)
    {
        size = target.size;
        return *this;
    }
        
    uint32_t LightDataSizeInfo::getBufferSize() const
    {
        const auto &pPhysicalDevice = pApp->getPhysicalDevice();
        const auto &properties = pPhysicalDevice->getProperties();
        const auto &minOffsetAlignment = static_cast<float>(properties.limits.minUniformBufferOffsetAlignment);
        return static_cast<uint32_t>(std::ceil(size / minOffsetAlignment) * minOffsetAlignment);
    }

    LightTextureInfo::LightTextureInfo(uint32_t bindingPriority
        , const Texture *pTexture
        , const Texture::ImageView *pImageView
        , const Texture::Sampler *pSampler
        , vk::ImageLayout imageLayout
        )
        : bindingPriority(bindingPriority)
        , pTexture(pTexture)
        , pImageView(pImageView)
        , pSampler(pSampler)
        , imageLayout(imageLayout)
    {

    }

    LightTextureInfo::LightTextureInfo(const LightTextureInfo &target)
        : bindingPriority(target.bindingPriority)
        , pTexture(target.pTexture)
        , pImageView(target.pImageView)
        , pSampler(target.pSampler)
        , imageLayout(target.imageLayout)
    {

    }
        
    LightTextureInfo &LightTextureInfo::operator=(const LightTextureInfo &target)
    {
        bindingPriority = target.bindingPriority;
        pTexture = target.pTexture;
        pImageView = target.pImageView;
        pSampler = target.pSampler;
        imageLayout = target.imageLayout;
        return *this;
    }

    LightData::LightData()
        : arrDataNames()
        , mapDatas()
        , mapDataCounts()
        , mapDataInfos()
        , mapDataSizeInfos()
        , arrTexNames()
        , mapTextures()
    {

    }

    const std::vector<std::string> LightData::getArrDataNames() const
    {
        return arrDataNames;
    }

    Bool32 LightData::hasData(std::string name) const
    {
        return hasValue<std::vector<Byte>>(name, mapDatas);
    }
        
    void LightData::removeData(std::string name)
    {
        removeValue(name, mapDatas, arrDataNames);
        removeValue(name, mapDataCounts);
        removeValue(name, mapDataInfos);
        removeValue(name, mapDataSizeInfos);
    }

    const LightDataInfo &LightData::getDataInfo(std::string name) const
    {
        return getValue(name, mapDataInfos);
    }

    const LightDataSizeInfo &LightData::getDataSizeInfo(std::string name) const
    {
        return getValue(name, mapDataSizeInfos);
    }

    void LightData::addData(const std::string name, const LightDataInfo &info, const LightDataSizeInfo &sizeInfo)
    {
        std::vector<Byte> temp(sizeInfo.size);
        addValue(name, temp, mapDatas, arrDataNames);
        addValue(name, 1u, mapDataCounts);
        addValue(name, info, mapDataInfos);
        addValue(name, sizeInfo, mapDataSizeInfos);
    }

    void LightData::addData(const std::string name, const LightDataInfo &info, void *src, uint32_t size)
    {
        std::vector<Byte> temp(size);
        if (size != 0u && src != nullptr) memcpy(temp.data(), src, size);
        addValue(name, temp, mapDatas, arrDataNames);
        addValue(name, 1u, mapDataCounts);
        addValue(name, info, mapDataInfos);
        LightDataSizeInfo sizeInfo = {
            size
        };
        addValue(name, sizeInfo, mapDataSizeInfos);
    }

    void LightData::getData(const std::string name, void *dst, uint32_t size, uint32_t offset) const
    {
        const auto &bytes = getValue(name, mapDatas);        
        if (offset + size > static_cast<uint32_t>(bytes.size()))
            throw std::range_error("Out range of the saved material data!");
        memcpy(dst, (char *)(bytes.data()) + offset, size);
    }

    void LightData::setData(const std::string name, const LightDataInfo &info, const LightDataSizeInfo &sizeInfo)
    {
        setValue(name, info, mapDataInfos);
        setValue(name, sizeInfo, mapDataSizeInfos);
    }

    void LightData::setData(const std::string name, void *src, uint32_t size, uint32_t offset)
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
                LightDataSizeInfo sizeInfo = {
                    newSize
                };
                setValue(name, sizeInfo, mapDataSizeInfos);
            }
            if(src) memcpy((char *)(mapDatas[name].data()) + offset, src, size);
            setValue(name, 1u, mapDataCounts);
        }
    }

    uint32_t LightData::getDataBaseSize(const std::string name) const
    {
        const auto& bytes = getValue(name, mapDatas);
        const auto& count = getValue(name, mapDataCounts);
        return static_cast<uint32_t>(bytes.size()) / count;
    }

    uint32_t LightData::getDataSize(const std::string name) const
    {
        const auto& bytes = getValue(name, mapDatas);
        return static_cast<uint32_t>(bytes.size());
    }

    void LightData::memoryCopyData(const std::string name
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

    const std::vector<std::string> LightData::getArrTextureNames() const
    {
        return arrTexNames;
    }

    Bool32 LightData::hasTexture(std::string name) const
    {
        return hasValue<LightTextureInfo>(name, mapTextures);
    }
        
    void LightData::addTexture(std::string name, const LightTextureInfo &texInfo)
    {
        addValue(name, texInfo, mapTextures, arrTexNames);
    }
        
    void LightData::removeTexture(std::string name)
    {
        removeValue(name, mapTextures, arrTexNames);
    }

    const LightTextureInfo &LightData::getTextureInfo(std::string name) const
    {
        return getValue(name, mapTextures, arrTexNames);
    }

    void LightData::setTexture(std::string name, const LightTextureInfo &texInfo)
    {
        getValue(name, mapTextures, arrTexNames) = texInfo;
    }


    LightExportTextureInfo::LightExportTextureInfo(const Texture *pTexture
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
        
    LightExportTextureInfo::LightExportTextureInfo(const LightExportTextureInfo &target)
        : pTexture(target.pTexture)
        , pImageView(target.pImageView)
        , pSampler(target.pSampler)
        , imageLayout(target.imageLayout)
    {

    }
        
    LightExportTextureInfo &LightExportTextureInfo::operator=(const LightExportTextureInfo &target)
    {
        pTexture = target.pTexture;
        pImageView = target.pImageView;
        pSampler = target.pSampler;
        imageLayout = target.imageLayout;
        return *this;
    }

    LightExportInfo::LightExportInfo(uint32_t dataSize
        , const void *pData
        , uint32_t textureCount
        , const LightExportTextureInfo *pTextureInfos
        )
        : dataSize(dataSize)
        , pData(pData)
        , textureCount(textureCount)
        , pTextureInfos(pTextureInfos)
    {

    }

    LightExportInfo::LightExportInfo(const LightExportInfo &target)
        : dataSize(target.dataSize)
        , pData(target.pData)
        , textureCount(target.textureCount)
        , pTextureInfos(target.pTextureInfos)
    {

    }
        
    LightExportInfo &LightExportInfo::operator=(const LightExportInfo &target)
    {
        dataSize = target.dataSize;
        pData = target.pData;
        textureCount = target.textureCount;
        pTextureInfos = target.pTextureInfos;
        return *this;
    }

    LightRegisterInfo::LightRegisterInfo(uint32_t dataSize
        , uint32_t textureCount
        )
        : dataSize(dataSize)
        , textureCount(textureCount)
    {

    }

    LightRegisterInfo::LightRegisterInfo(const LightRegisterInfo &target)
        : dataSize(target.dataSize)
        , textureCount(target.textureCount)
    {

    }
        
    LightRegisterInfo &LightRegisterInfo::operator=(const LightRegisterInfo &target)
    {
        dataSize = target.dataSize;
        textureCount = target.textureCount;
        return *this;
    }

    LightDepthRenderInfo::LightDepthRenderInfo(uint32_t renderCount
        , const BaseProjector *const *pProjectors
        , const PreDepthTarget *const *pDepthTargets
        )
        : renderCount(renderCount)
        , pProjectors(pProjectors)
        , pDepthTargets(pDepthTargets)
    {

    }


//Light
    BaseLight::BaseLight()
        : Base(BaseType::SCENE_OBJECT)
        , m_data()
        , m_dataChanged(VG_FALSE)
        , m_dataContentChanged(VG_FALSE)
        , m_dataContentChanges()
        , m_textureChanged(VG_FALSE)
        , m_sortDataSet(_compareDataInfo)
        , m_dataMemoryBuffer()
        , m_sortTexInfosSet(_compareTextureInfo)
        , m_textureInfos()
    {

    }

    void BaseLight::apply()
    {
        _apply();
    }

    LightExportInfo BaseLight::getExportInfo() const
    {
        LightExportInfo exportInfo;
        exportInfo.dataSize = static_cast<uint32_t>(m_dataMemoryBuffer.size());
        exportInfo.pData = static_cast<const void *>(m_dataMemoryBuffer.data());
        exportInfo.textureCount = static_cast<uint32_t>(m_textureInfos.size());
        exportInfo.pTextureInfos = m_textureInfos.data();
        return exportInfo;
    }

    BaseLight::DataSortInfo::DataSortInfo(std::string name
        , uint32_t layoutPriority
        , uint32_t size
        , uint32_t bufferSize
        )
        : name(name)
        , layoutPriority(layoutPriority)
        , size(size)
        , bufferSize(bufferSize)
    {

    }

    Bool32 BaseLight::_compareDataInfo(const DataSortInfo &item1, const DataSortInfo &item2)
    {
        return item1.layoutPriority < item2.layoutPriority;
    }

    BaseLight::TextureSortInfo::TextureSortInfo(std::string name
        , uint32_t bindingPriority
        , const LightTextureInfo *pTextureInfo
        )
        : name(name)
        , bindingPriority(bindingPriority)
        , pTextureInfo(pTextureInfo)
    {

    }

    Bool32 BaseLight::_compareTextureInfo(const TextureSortInfo &item1, const TextureSortInfo &item2)
    {
        return item1.bindingPriority < item2.bindingPriority;
    }

    void BaseLight::_apply()
    {
        const auto &data = m_data;
        if (m_dataChanged)
        {
            //Construct data buffer.
            m_sortDataSet.clear();
            auto const &arrDataNames = data.arrDataNames;
            if (arrDataNames.size() > 0)
            {
                uint32_t totalBufferSize = 0u;
                for (const auto &name : arrDataNames)
                {
                    const auto &dataInfo = data.getDataInfo(name);
                    const auto &dataSizeInfo = data.getDataSizeInfo(name);
                    DataSortInfo sortInfo = {
                        name, 
                        dataInfo.layoutPriority,
                        dataSizeInfo.size,
                        dataSizeInfo.getBufferSize(),
                    };
                    m_sortDataSet.insert(sortInfo);
                    totalBufferSize += sortInfo.bufferSize;
                }

                auto &dataMemoryBuffer = m_dataMemoryBuffer;
                dataMemoryBuffer.resize(totalBufferSize);
                uint32_t offset = 0u;
                for (const auto &info : m_sortDataSet)
                {
                    data.getData(info.name, dataMemoryBuffer.data() + offset, info.size, 0u);
                    offset += info.bufferSize;
                }
            }

            m_dataChanged = VG_FALSE;
            m_dataContentChanged = VG_FALSE;
            m_dataContentChanges.clear();
        }

        if (m_dataContentChanged)
        {
            //Update data buffer.
            uint32_t index = 0u;
            uint32_t offset = 0u;
            auto &dataMemoryBuffer = m_dataMemoryBuffer;
            for (const auto &info : m_sortDataSet)
            {
                if (m_dataContentChanges.count(info.name) != 0)
                {
                    data.getData(info.name, dataMemoryBuffer.data() + offset, info.size, 0u);
                }
                offset += info.bufferSize;
            }
            m_dataContentChanged = VG_FALSE;
            m_dataContentChanges.clear();
        }

        if (m_textureChanged)
        {
            auto &sortInfos = m_sortTexInfosSet;
            sortInfos.clear();
            const auto &arrTextureNames = data.arrTexNames;
            for (const auto &name : arrTextureNames)
            {
                const auto &texInfo = data.getTextureInfo(name);
                TextureSortInfo sortInfo = {
                    name,
                    texInfo.bindingPriority,
                    &texInfo,
                };
                sortInfos.insert(sortInfo);
            }

            auto &textureInfos = m_textureInfos;
            textureInfos.resize(sortInfos.size());
            uint32_t index = 0u;
            for (const auto &info : sortInfos)
            {
                const auto &pTextureInfo = info.pTextureInfo;
                textureInfos[index].pTexture = pTextureInfo->pTexture;
                textureInfos[index].pImageView = pTextureInfo->pImageView;
                textureInfos[index].pSampler = pTextureInfo->pSampler;
                textureInfos[index].imageLayout = pTextureInfo->imageLayout;
                ++index;
            }
        }
    }
}