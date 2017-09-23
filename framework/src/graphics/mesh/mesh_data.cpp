#include "graphics/mesh/mesh_data.hpp"

namespace kgs
{
	uint32_t MeshData::getDataBaseTypeSize(const DataType dataType)
	{
		switch (dataType)
		{
		case DataType::FLOAT_ARRAY:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::FLOAT_ARRAY>::BaseType));
			break;
		}
		case DataType::INT_ARRAY:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::INT_ARRAY>::BaseType));
			break;
		}
		case DataType::VECTOR_2_ARRAY:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_2_ARRAY>::BaseType));
			break;
		}
		case DataType::VECTOR_3_ARRAY:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_3_ARRAY>::BaseType));
			break;
		}
		case DataType::VECTOR_4_ARRAY:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_4_ARRAY>::BaseType));
			break;
		}
		case DataType::COLOR_32_ARRAY:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::COLOR_32_ARRAY>::BaseType));
			break;
		}
		default:
		    throw std::runtime_error("Invalid data type for getting memeory size used by its base type.");
		}
	}

	uint32_t MeshData::getDataValueSize(std::string name, DataType dataType)
	{
		switch (dataType)
		{
		case DataType::FLOAT_ARRAY:
		{
			auto arr = getValue(name, mapFloatArrays);
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::FLOAT_ARRAY>::BaseType) * arr.size());
			break;
		}
		case DataType::INT_ARRAY:
		{
			auto arr = getValue(name, mapIntArrays);
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::INT_ARRAY>::BaseType) * arr.size());
			break;
		}
		case DataType::VECTOR_2_ARRAY:
		{
			auto arr = getValue(name, mapVector2Arrays);
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_2_ARRAY>::BaseType) * arr.size());
			break;
		}
		case DataType::VECTOR_3_ARRAY:
		{
			auto arr = getValue(name, mapVector3Arrays);
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_3_ARRAY>::BaseType) * arr.size());
			break;
		}
		case DataType::VECTOR_4_ARRAY:
		{
			auto arr = getValue(name, mapVector4Arrays);
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_4_ARRAY>::BaseType) * arr.size());
			break;
		}
		case DataType::COLOR_32_ARRAY:
		{
			auto arr = getValue(name, mapColor32Arrays);
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::COLOR_32_ARRAY>::BaseType) * arr.size());
			break;
		}
		default:
			throw std::runtime_error("Invalid data type for getting its used memory size.");
		}
	}

	void MeshData::memCopyDataValue(std::string name, DataType dataType, void* dst, uint32_t offset, uint32_t elementStart, uint32_t maxElementCount)
	{
		char *ptr = static_cast<char *>(dst);
		ptr += offset;
		switch (dataType)
		{
		case DataType::FLOAT_ARRAY:
		{
			auto arr = getValue(name, mapFloatArrays);
			uint32_t finalElementCount = std::max(0u, std::min(static_cast<uint32_t>(arr.size()) - elementStart, maxElementCount));
			if (finalElementCount == 0) break;
			std::memcpy(ptr, arr.data() + elementStart, sizeof(DataTypeInfo<DataType::FLOAT_ARRAY>::BaseType) * finalElementCount);
			break;
		}
		case DataType::INT_ARRAY:
		{
			auto arr = getValue(name, mapIntArrays);
			uint32_t finalElementCount = std::max(0u, std::min(static_cast<uint32_t>(arr.size()) - elementStart, maxElementCount));
			if (finalElementCount == 0) break;
			std::memcpy(ptr, arr.data() + elementStart, sizeof(DataTypeInfo<DataType::INT_ARRAY>::BaseType) * finalElementCount);
			break;
		}
		case DataType::VECTOR_2_ARRAY:
		{
			auto arr = getValue(name, mapVector2Arrays);
			uint32_t finalElementCount = std::max(0u, std::min(static_cast<uint32_t>(arr.size()) - elementStart, maxElementCount));
			if (finalElementCount == 0) break;
			std::memcpy(ptr, arr.data() + elementStart, sizeof(DataTypeInfo<DataType::VECTOR_2_ARRAY>::BaseType) * finalElementCount);
			break;
		}
		case DataType::VECTOR_3_ARRAY:
		{
			auto arr = getValue(name, mapVector3Arrays);
			uint32_t finalElementCount = std::max(0u, std::min(static_cast<uint32_t>(arr.size()) - elementStart, maxElementCount));
			if (finalElementCount == 0) break;
			std::memcpy(ptr, arr.data() + elementStart, sizeof(DataTypeInfo<DataType::VECTOR_3_ARRAY>::BaseType) * finalElementCount);
			break;
		}
		case DataType::VECTOR_4_ARRAY:
		{
			auto arr = getValue(name, mapVector4Arrays);
			uint32_t finalElementCount = std::max(0u, std::min(static_cast<uint32_t>(arr.size()) - elementStart, maxElementCount));
			if (finalElementCount == 0) break;
			std::memcpy(ptr, arr.data() + elementStart, sizeof(DataTypeInfo<DataType::VECTOR_4_ARRAY>::BaseType) * finalElementCount);
			break;
		}
		case DataType::COLOR_32_ARRAY:
		{
			auto arr = getValue(name, mapColor32Arrays);
			uint32_t finalElementCount = std::max(0u, std::min(static_cast<uint32_t>(arr.size()) - elementStart, maxElementCount));
			if (finalElementCount == 0) break;
			std::memcpy(ptr, arr.data() + elementStart, sizeof(DataTypeInfo<DataType::COLOR_32_ARRAY>::BaseType) * finalElementCount);
			break;
		}
		default:
			throw std::runtime_error("Invalid data type for copying its data to memory.");
		}
	}

	//------------------this used to eliminate ide warning------------------------------
	template <MeshData::DataType type>
	typename MeshData::DataTypeInfo<type>::ValueType& MeshData::getDataValue(std::string name) { return {}; }

	template <MeshData::DataType type>
	void MeshData::setDataValue(std::string name, typename MeshData::DataTypeInfo<type>::ValueType& value) {}
	//-----------------this used to eliminate ide warning----------------

	template<>
	std::vector<float>& MeshData::getDataValue<MeshData::DataType::FLOAT_ARRAY>(std::string name)
	{
		return getValue(name, mapFloatArrays);
	}

	template<>
	void MeshData::setDataValue<MeshData::DataType::FLOAT_ARRAY>(std::string name, std::vector<float>& value)
	{
		setValue(name, value, mapFloatArrays);
	}

	template<>
	std::vector<int32_t>& MeshData::getDataValue<MeshData::DataType::INT_ARRAY>(std::string name)
	{
		return getValue(name, mapIntArrays);
	}

	template<>
	void MeshData::setDataValue<MeshData::DataType::INT_ARRAY>(std::string name, std::vector<int32_t>& value)
	{
		setValue(name, value, mapIntArrays);
	}

	template<>
	std::vector<Vector2>& MeshData::getDataValue<MeshData::DataType::VECTOR_2_ARRAY>(std::string name)
	{
		return getValue(name, mapVector2Arrays);
	}

	template<>
	void MeshData::setDataValue<MeshData::DataType::VECTOR_2_ARRAY>(std::string name, std::vector<Vector2>& value)
	{
		setValue(name, value, mapVector2Arrays);
	}

	template<>
	std::vector<Vector3>& MeshData::getDataValue<MeshData::DataType::VECTOR_3_ARRAY>(std::string name)
	{
		return getValue(name, mapVector3Arrays);
	}

	template<>
	void MeshData::setDataValue<MeshData::DataType::VECTOR_3_ARRAY>(std::string name, std::vector<Vector3>& value)
	{
		setValue(name, value, mapVector3Arrays);
	}

	template<>
	std::vector<Vector4>& MeshData::getDataValue<MeshData::DataType::VECTOR_4_ARRAY>(std::string name)
	{
		return getValue(name, mapVector4Arrays);
	}

	template<>
	void MeshData::setDataValue<MeshData::DataType::VECTOR_4_ARRAY>(std::string name, std::vector<Vector4>& value)
	{
		setValue(name, value, mapVector4Arrays);
	}

	template<>
	std::vector<Color32>& MeshData::getDataValue<MeshData::DataType::COLOR_32_ARRAY>(std::string name)
	{
		return getValue(name, mapColor32Arrays);
	}

	template<>
	void MeshData::setDataValue<MeshData::DataType::COLOR_32_ARRAY>(std::string name, std::vector<Color32>& value)
	{
		setValue(name, value, mapColor32Arrays);
	}
}