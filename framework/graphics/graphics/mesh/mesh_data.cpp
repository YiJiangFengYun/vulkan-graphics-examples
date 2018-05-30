#include "graphics/mesh/mesh_data.hpp"

namespace vg
{
    MeshData::DataInfo::DataInfo()
        : dataType()
        , slot()
    {

    }

    MeshData::DataInfo::DataInfo(MeshData::DataType dataType
        , uint32_t slot
        )
        : dataType(dataType)
        , slot(slot)
    {

    }

    MeshData::DataInfo::DataInfo(const DataInfo &target)
        : dataType(target.dataType)
        , slot(target.slot)
    {

    }

    MeshData::DataInfo &MeshData::DataInfo::operator=(const DataInfo &target)
    {
        dataType = target.dataType;
        slot = target.slot;
        return *this;
    }

    Bool32 MeshData::DataInfo::operator==(const DataInfo &target) const
    {
        return slot == target.slot && dataType == target.dataType;
    }

    Bool32 MeshData::DataInfo::operator<(const DataInfo &target) const
    {
        return slot < target.slot;
    }

    MeshData::Data::Data(DataInfo info
        , std::vector<Byte> data
        , uint32_t count
        )
        : info(info)
        , data(data)
        , count(count)
    {

    }

    const std::set<SlotType> &MeshData::Datas::getSlots() const
    {
        return datas.getSlots();
    }

    Bool32 MeshData::Datas::hasData(const SlotType &slot) const
    {
        return datas.hasValue(slot);
    }
        
    void MeshData::Datas::removeData(const SlotType &slot)
    {
        datas.removeValue(slot);
    }

    uint32_t MeshData::Datas::getDataCount(const SlotType &slot) const
    {
        return datas.getValue(slot).count;
    }

    const MeshData::DataInfo &MeshData::Datas::getDataInfo(const SlotType &slot) const
    {
        return datas.getValue(slot).info;
    }

    void MeshData::Datas::addData(const SlotType &slot, const DataInfo &info, void *src, uint32_t size)
    {
        std::vector<Byte> temp(size);
        if (size != 0u && src != nullptr) memcpy(temp.data(), src, size);
        Data tempData(info, temp, 1u);
        datas.addValue(slot, std::move(tempData));
    }

    void MeshData::Datas::getData(const SlotType &slot, void *dst, uint32_t size, uint32_t offset) const
    {
        const auto &bytes = datas.getValue(slot).data;
        if (offset + size > static_cast<uint32_t>(bytes.size()))
            throw std::range_error("Out range of the saved material data!");
        memcpy(dst, (char *)(bytes.data()) + offset, size);
    }

    void MeshData::Datas::setData(const SlotType &slot, void *src, uint32_t size, uint32_t offset)
    {
        auto &bytes = datas.getValue(slot).data;
        if (offset + size > static_cast<uint32_t>(bytes.size()))
            bytes.resize(offset + size);
        if(src) memcpy(bytes.data() + offset, src, size);
    }

    uint32_t MeshData::Datas::getDataBaseSize(const SlotType &slot) const
    {
        const auto& bytes = datas.getValue(slot).data;
        const auto& count = datas.getValue(slot).count;
        return static_cast<uint32_t>(bytes.size()) / count;
    }
    
    uint32_t MeshData::Datas::getDataSize(const SlotType &slot) const
    {
        const auto& bytes = datas.getValue(slot).data;
        return static_cast<uint32_t>(bytes.size());
    }

    void MeshData::Datas::memoryCopyData(const SlotType &slot
        , void* dst
        , uint32_t offset
        , uint32_t elementStart
        , uint32_t maxElementCount) const
    {
        const auto& bytes = datas.getValue(slot).data;
        const auto& count = datas.getValue(slot).count;
        uint32_t baseSize = static_cast<uint32_t>(bytes.size()) / count;
        char *ptr = static_cast<char *>(dst);
        ptr += offset;
        uint32_t finalElementCount = std::max(0u, std::min(count - elementStart, maxElementCount));
        if (finalElementCount == 0) return;
        uint32_t srcOffset = elementStart * baseSize;
        uint32_t srcSize = finalElementCount * baseSize;
        std::memcpy(ptr, bytes.data() + srcOffset, srcSize);
    }

    MeshData::MeshData()
        : datas()
    {

    }

    uint32_t MeshData::getDataBaseSize(const DataType dataType)
    {
        switch (dataType)
        {
        case DataType::FLOAT_ARRAY:
        {
            return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::FLOAT_ARRAY>::BaseType));
        }
        case DataType::INT_ARRAY:
        {
            return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::INT_ARRAY>::BaseType));
        }
        case DataType::VECTOR_2_ARRAY:
        {
            return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_2_ARRAY>::BaseType));
        }
        case DataType::VECTOR_3_ARRAY:
        {
            return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_3_ARRAY>::BaseType));
        }
        case DataType::VECTOR_4_ARRAY:
        {
            return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_4_ARRAY>::BaseType));
        }
        case DataType::COLOR_32_ARRAY:
        {
            return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::COLOR_32_ARRAY>::BaseType));
        }
        case DataType::COLOR_ARRAY:
        {
            return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::COLOR_ARRAY>::BaseType));
        }
        default:
            throw std::runtime_error("Invalid data type for getting memeory size used by its base type.");
        }
    }

    void MeshData::memoryCopyData(DataType type
            , const SlotType &slot
            , void* dst
            , uint32_t offset
            , uint32_t elementStart
            , uint32_t maxElementCount) const
    {
        datas[static_cast<size_t>(type)].memoryCopyData(slot, dst, offset, elementStart, maxElementCount);
    }
    
}