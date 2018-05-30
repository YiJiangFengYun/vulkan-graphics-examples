namespace vg
{
    template<typename T>
    void MeshData::Datas::addData(const SlotType &slot, const DataInfo &info, const T &value)
    {
        std::vector<Byte> temp(sizeof(T));
        memcpy(temp.data(), &value, sizeof(T));
        Data tempData(info, temp, 1u);
        datas.addValue(slot, std::move(tempData));
    }

    template <typename T>
    T MeshData::Datas::getData(const SlotType &slot) const
    {
        const auto& bytes = datas.getValue().data;
        T t;
        memcpy(&t, bytes.data(), sizeof(T));
        return t;
    }

    template<typename T>
    void MeshData::Datas::setData(const SlotType &slot, const T &value)
    {
        auto &bytes = datas.getValue(slot).data;
        if (bytes.size() < sizeof(T)) bytes.resize(sizeof(T));
        memcpy(bytes.data(), &value, sizeof(T));
    }

    template<typename T>
    void MeshData::Datas::addData(const SlotType &slot, const DataInfo &info, const std::vector<T> &values)
    {
        std::vector<Byte> temp(sizeof(T) * values.size());
        memcpy(temp.data(), values.data(), temp.size());
        Data tempData(info, temp, static_cast<uint32_t>(values.size()));
        datas.addValue(slot, std::move(tempData));
    }

    template <typename T>
    std::vector<T> MeshData::Datas::getData(const SlotType &slot, const uint32_t count) const
    {
        const auto& bytes = datas.getValue(slot).data;
        std::vector<T> ts(count);
        memcpy(ts.data(), bytes.data(), sizeof(T) * count);
        return ts;
    }

    template<typename T>
    void MeshData::Datas::setData(const SlotType &slot, const std::vector<T> &values)
    {
        auto &bytes = datas.getValue(slot).data;
        uint32_t srcSize = static_cast<uint32_t>(sizeof(T) * values.size());
        if (srcSize > static_cast<uint32_t>(bytes.size()))
            bytes.resize(srcSize);
        memcpy(bytes.data(), values.data(), srcSize);
        datas.getValue(slot).count = static_cast<uint32_t>(values.size());
    }

    template<typename T>
    void MeshData::Datas::addData(const SlotType &slot, const DataInfo &info, const T * const pSrc, const uint32_t count)
    {
        std::vector<Byte> temp(sizeof(T) * count);
        if (count != 0u && pSrc != nullptr) memcpy(temp.data(), pSrc, temp.size());

        Data tempData(info, temp, count);
        datas.addValue(slot, std::move(tempData));
    }

    template<typename T>
    void MeshData::Datas::getData(const SlotType &slot, const T * const pDst, const uint32_t count)
    {
        const auto& bytes = datas.getValue(slot).data;
        size_t size = sizeof(T) * count;
        memcpy(pDst, bytes.data(), size);
    }

    template<typename T>
    void MeshData::Datas::setData(const SlotType &slot, const T * const pSrc, const uint32_t count)
    {
        auto &bytes = datas.getValue(slot).data;
        uint32_t srcSize = static_cast<uint32_t>(sizeof(T)) * count;
        if (srcSize > static_cast<uint32_t>(bytes.size()))
            bytes.resize(srcSize);
        if (srcSize > 0) memcpy(bytes.data(), pSrc, srcSize);
        datas.getValue(slot).count = count;
    }

    template <MeshData::DataType type>
    Bool32 MeshData::hasData(const SlotType &slot) const
    {
        return datas[static_cast<size_t>(type)].hasData(slot);
    }

    template <MeshData::DataType type>
    void MeshData::addData(const SlotType &slot, const DataInfo &info, const typename DataTypeInfo<type>::ValueType& value)
    {
        datas[static_cast<size_t>(type)].addData(slot, info, value);
    }

    template <MeshData::DataType type>
    void MeshData::removeData(const SlotType &slot)
    {
        datas[static_cast<size_t>(type)].removeData(slot);
    }

    template <MeshData::DataType type>
    const typename MeshData::DataTypeInfo<type>::ValueType MeshData::getData(const SlotType &slot) const
    {
        auto count = datas[static_cast<size_t>(type)].getDataCount(slot);
        return datas[static_cast<size_t>(type)].getData<typename DataTypeInfo<type>::BaseType>(slot, count);
    }

    template <MeshData::DataType type>
    void MeshData::setData(const SlotType &slot, const typename DataTypeInfo<type>::ValueType& value)
    {
        datas[static_cast<size_t>(type)].setData<typename DataTypeInfo<type>::BaseType>(slot, value);
    }

    template <MeshData::DataType type>
    uint32_t MeshData::getDataBaseSize()
    {
        return static_cast<uint32_t>(sizeof(DataTypeInfo<type>::BaseType));
    }

    template <MeshData::DataType type>
    uint32_t MeshData::getDataSize(const SlotType &slot) const
    {
        datas[static_cast<size_t>(type)].getDataSize(slot);
    }

    template <MeshData::DataType type>
    void MeshData::memoryCopyData(const SlotType &slot
        , void* dst
        , uint32_t offset
        , uint32_t elementStart
        , uint32_t maxElementCount) const
    {
        datas[static_cast<size_t>(type)].memoryCopyData(slot, dst, offset, elementStart, maxElementCount);
    }
} //vg