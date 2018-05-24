#include "graphics/mesh/mesh_data.hpp"

namespace vg
{
	MeshData::DataInfo::DataInfo()
	{

	}

	MeshData::DataInfo::DataInfo(std::string name,
		MeshData::DataType dataType,
		uint32_t bindingPriority
		)
		: name(name)
		, dataType(dataType)
		, bindingPriority(bindingPriority)
	{

	}

	MeshData::DataInfo::DataInfo(const DataInfo &target)
		: name(target.name)
		, dataType(target.dataType)
		, bindingPriority(target.bindingPriority)
	{

	}

	MeshData::DataInfo &MeshData::DataInfo::operator=(const DataInfo &target)
	{
		name = target.name;
		dataType = target.dataType;
		bindingPriority = target.bindingPriority;
		return *this;
	}


	MeshData::DataInfo::DataInfo(const DataInfo &&target)
		: name(target.name)
		, dataType(target.dataType)
		, bindingPriority(target.bindingPriority)
	{

	}

	Bool32 MeshData::DataInfo::operator==(const DataInfo &target) const
	{
		return name == target.name && dataType == target.dataType && bindingPriority == target.bindingPriority;
	}

	Bool32 MeshData::DataInfo::operator<(const DataInfo &target) const
	{
		return bindingPriority < target.bindingPriority;
	}

	const std::vector<std::string> MeshData::getArrDataNames() const
	{
		return arrDataNames;
	}

	Bool32 MeshData::hasData(std::string name) const
	{
		return hasValue<std::vector<Byte>>(name, mapDatas);
	}
		
	void MeshData::removeData(std::string name)
	{
		removeValue(name, mapDatas, arrDataNames);
		removeValue(name, mapDataCounts);
		removeValue(name, mapDataInfos);
	}

	const MeshData::DataInfo &MeshData::getDataInfo(std::string name) const
	{
		return getValue(name, mapDataInfos);
	}

	void MeshData::addData(const std::string name, const DataInfo &info, void *src, uint32_t size)
	{
		std::vector<Byte> temp(size);
		if (size != 0u && src != nullptr) memcpy(temp.data(), src, size);
		addValue(name, temp, mapDatas, arrDataNames);
		addValue(name, 1u, mapDataCounts);
		addValue(name, info, mapDataInfos);
	}

	void MeshData::getData(const std::string name, void *dst, uint32_t size, uint32_t offset) const
	{
		const auto &bytes = getValue(name, mapDatas);		
		if (offset + size > static_cast<uint32_t>(bytes.size()))
		    throw std::range_error("Out range of the saved material data!");
		memcpy(dst, (char *)(bytes.data()) + offset, size);
	}

	void MeshData::setData(const std::string name, void *src, uint32_t size, uint32_t offset)
	{
		const auto& iterator = mapDatas.find(name);
		if (iterator == mapDatas.cend())
		{
			throw std::runtime_error("Map don't has item whose key: " + name);
		}
		else
		{
			if (offset + size > static_cast<uint32_t>(mapDatas[name].size()))
		        mapDatas[name].resize(offset + size);
		    if(src) memcpy((char *)(mapDatas[name].data()) + offset, src, size);
		}
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

	uint32_t MeshData::getDataBaseSize(const std::string name) const
	{
		const auto& bytes = getValue(name, mapDatas);
		const auto& count = getValue(name, mapDataCounts);
		return static_cast<uint32_t>(bytes.size()) / count;
	}

	uint32_t MeshData::getDataSize(const std::string name) const
	{
		const auto& bytes = getValue(name, mapDatas);
		return static_cast<uint32_t>(bytes.size());
	}

	void MeshData::memoryCopyData(const std::string name
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
}