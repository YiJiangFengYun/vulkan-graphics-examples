namespace vg
{
    template<typename T>
    void PassDatas::addData(const SlotType &slot, const PassDataInfo &info, const T &value)
    {
        std::vector<Byte> temp(sizeof(T));
        memcpy(temp.data(), &value, sizeof(T));
        PassDataSizeInfo sizeInfo = {
            static_cast<uint32_t>(sizeof(T)),
        };
        Data tempData(info, sizeInfo, temp, 1u);
        datas.addValue(slot, std::move(tempData));
    }

    template <typename T>
    T PassDatas::getData(const SlotType &slot) const
    {
        const auto& bytes = datas.getValue().data;
        T t;
        memcpy(&t, bytes.data(), sizeof(T));
        return t;
    }

    template<typename T>
    void PassDatas::setData(const SlotType &slot, const T &value)
    {
        auto &value = datas.getValue(slot);
        auto &bytes = value.data;
        auto size = static_cast<uint32_t>(sizeof(T));
        if (bytes.size() < size) bytes.resize(size);
        memcpy(bytes.data(), &value, size);
        value.count = 1u;
        value.sizeInfo.size = size;
    }

    template<typename T>
    void PassDatas::addData(const SlotType &slot, const PassDataInfo &info, const std::vector<T> &values)
    {
        uint32_t size = static_cast<uint32_t>(sizeof(T) * values.size());
        std::vector<Byte> temp(size);
        memcpy(temp.data(), values.data(), temp.size());
        PassDataSizeInfo sizeInfo = {
            size,
        };
        Data tempData(info, sizeInfo, temp, static_cast<uint32_t>(values.size()));
        datas.addValue(slot, std::move(tempData));
    }

    template <typename T>
    std::vector<T> PassDatas::getData(const SlotType &slot, const uint32_t count) const
    {
        const auto& bytes = datas.getValue(slot).data;
        std::vector<T> ts(count);
        memcpy(ts.data(), bytes.data(), sizeof(T) * count);
        return ts;
    }

    template<typename T>
    void PassDatas::setData(const SlotType &slot, const std::vector<T> &values)
    {
        auto &value = datas.getValue(slot);
        auto &bytes = value.data;
        uint32_t srcSize = static_cast<uint32_t>(sizeof(T) * values.size());
        if (srcSize > static_cast<uint32_t>(bytes.size()))
        {
            bytes.resize(srcSize);
            value.sizeInfo.size = srcSize;
        }
        memcpy(bytes.data(), values.data(), srcSize);
        value.count = static_cast<uint32_t>(values.size());
    }

    template<typename T>
    void PassDatas::addData(const SlotType &slot, const PassDataInfo &info, const T * const pSrc, const uint32_t count)
    {
        uint32_t size = static_cast<uint32_t>(sizeof(T) * count;
        std::vector<Byte> temp(size);
        if (count != 0u && pSrc != nullptr)memcpy(temp.data(), pSrc, size);
        PassDataSizeInfo sizeInfo = {
            size,
        };
        Data tempData(info, sizeInfo, temp, count);
        datas.addValue(slot, std::move(tempData));
    }

    template<typename T>
    void PassDatas::getData(const std::string name, const T * const pDst, const uint32_t count)
    {
        const auto& bytes = datas.getValue(slot).data;
        size_t size = sizeof(T) * count;
        memcpy(pDst, bytes.data(), size);
    }

    template<typename T>
    void PassDatas::setData(const std::string name, const T * const pSrc, const uint32_t count)
    {
        auto &value = datas.getValue(slot);
        auto &bytes = value.data;
        uint32_t srcSize = static_cast<uint32_t>(sizeof(T)) * count;
        if (srcSize > static_cast<uint32_t>(bytes.size())) {
            bytes.resize(srcSize);
            value.sizeInfo.size = srcSize;
        }
        if (srcSize > 0) memcpy(bytes.data(), pSrc, srcSize);
        value.count = count;
    }
}