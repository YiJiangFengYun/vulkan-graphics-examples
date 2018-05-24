namespace vg
{
    template<typename T>
	void MeshData::Data::addData(const std::string name, const DataInfo &info, const T &value)
	{
		std::vector<Byte> temp(sizeof(T));
		memcpy(temp.data(), &value, sizeof(T));
		addValue(name, mapDatas, arrDataNames);
		addValue(name, 1u, mapDataCounts);
		addValue(name, info, mapDataInfos);
	}

	template <typename T>
	T MeshData::Data::getData(const std::string name) const
	{
		const auto& bytes = getValue(name, mapDatas);
		T t;
		memcpy(&t, bytes.data(), sizeof(T));
		return t;
	}

	template<typename T>
	void MeshData::Data::setData(const std::string name, const T &value)
	{
		std::vector<Byte> temp(sizeof(T));
		memcpy(temp.data(), &value, sizeof(T));
		setValue(name, temp, mapDatas);
	}

	template<typename T>
	void MeshData::Data::addData(const std::string name, const DataInfo &info, const std::vector<T> &values)
	{
		std::vector<Byte> temp(sizeof(T) * values.size());
		memcpy(temp.data(), values.data(), temp.size());
		addValue(name, temp, mapDatas, arrDataNames);
		addValue(name, values.size(), mapDataCounts);
		addValue(name, info, mapDataInfos);
	}

	template <typename T>
	std::vector<T> MeshData::Data::getData(const std::string name, const uint32_t count) const
	{
		const auto& bytes = getValue(name, mapDatas);
		std::vector<T> ts(count);
		memcpy(ts.data(), bytes.data(), sizeof(T) * count);
		return ts;
	}

	template<typename T>
	void MeshData::Data::setData(const std::string name, const std::vector<T> &values)
	{
		std::vector<Byte> temp(sizeof(T) * values.size());
		memcpy(temp.data(), values.data(), temp.size());
		setValue(name, temp, mapDatas);
		setValue(name, values.size(), mapDataCounts);
	}

	template<typename T>
	void MeshData::Data::addData(const std::string name, const DataInfo &info, const T * const pSrc, const uint32_t count)
	{
		std::vector<Byte> temp(sizeof(T) * count);
		if (count != 0u && pSrc != nullptr) memcpy(temp.data(), pSrc, temp.size());
		addValue(name, temp, mapDatas, arrDataNames);
		addValue(name, count, mapDataCounts);
		addValue(name, info, mapDataInfos);
	}

	template<typename T>
	void MeshData::Data::getData(const std::string name, const T * const pDst, const uint32_t count)
	{
		const auto& bytes = getValue(name, mapDatas);
		size_t size = sizeof(T) * count;
		memcpy(pDst, bytes.data(), size);
	}

	template<typename T>
	void MeshData::Data::setData(const std::string name, const T * const pSrc, const uint32_t count)
	{
		std::vector<Byte> temp(sizeof(T) * count);
		if (count != 0u && pSrc != nullptr) memcpy(temp.data(), pSrc, temp.size());
		setValue(name, temp, mapDatas);
		setValue(name, count, mapDataCounts);
	}

	template <MeshData::DataType type>
	Bool32 MeshData::hasData(std::string name) const
	{
		return return datas[static_cast<size_t>(type)].hasData(name);
	}

	template <MeshData::DataType type>
	void MeshData::addData(std::string name, const DataInfo &info, const typename DataTypeInfo<type>::ValueType& value)
	{
		datas[static_cast<size_t>(type)].addData(name, info, value);
	}

	template <MeshData::DataType type>
	void MeshData::removeData(std::string name)
	{
		datas[static_cast<size_t>(type)].removeData(name);
	}

	template <MeshData::DataType type>
	const typename MeshData::DataTypeInfo<type>::ValueType& MeshData::getData(std::string name) const
	{
		return datas[static_cast<size_t>(type)].getData(name);
	}

	template <MeshData::DataType type>
	void MeshData::setData(std::string name, const typename DataTypeInfo<type>::ValueType& value)
	{
		datas[static_cast<size_t>(type)].setData(name, value);
	}

	template <MeshData::DataType type>
	uint32_t MeshData::getDataBaseSize()
	{
		return static_cast<uint32_t>(sizeof(DataTypeInfo<type>::BaseType));
	}

	template <MeshData::DataType type>
	uint32_t MeshData::getDataSize(const std::string name) const
	{
		datas[static_cast<size_t>(type)].getDataSize(name);
	}

    template <MeshData::DataType type>
	void MeshData::memoryCopyData(const std::string name
		, void* dst
		, uint32_t offset
		, uint32_t elementStart
		, uint32_t maxElementCount) const
	{
		datas[static_cast<size_t>(type)].memoryCopyData(name, dst, offset, elementStart, maxElementCount);
	}
} //vg