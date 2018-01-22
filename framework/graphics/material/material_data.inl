namespace vg
{
	template <typename T>
	T MaterialData::getDataValue(std::string name) const
	{
		const auto& bytes = getValue(name, mapDatas);
		T t;
		memcpy(&t, bytes.data(), sizeof(T));
		return t;
	}

	template <typename T>
	std::vector<T> MaterialData::getDataValue(std::string name, uint32_t count) const
	{
		const auto& bytes = getValue(name, mapDatas);
		std::vector<T> ts(count);
		memcpy(ts.data(), bytes.data(), sizeof(T) * count);
		return ts;
	}

	template<typename T>
	void MaterialData::setDataValue(std::string name, const T &value)
	{
		mapDatas[name].resize(sizeof(T));
		memcpy(mapDatas[name].data(), &value, sizeof(T));
		auto iterator = std::find(arrDataNames.begin(), arrDataNames.end(), name);
		if (iterator == arrDataNames.end())
		{
			arrDataNames.push_back(name);
		}
		mapDataCounts[name] = 1u;
	}

	template<typename T>
	void MaterialData::setDataValue(std::string name, const std::vector<T> &values)
	{
		size_t size = sizeof(T) * values.size();
		mapDatas[name].resize(size);
		memcpy(mapDatas[name].data(), &values, size);
		auto iterator = std::find(arrDataNames.begin(), arrDataNames.end(), name);
		if (iterator == arrDataNames.end())
		{
			arrDataNames.push_back(name);
		}
		mapDataCounts[name] = values.size();
	}
}