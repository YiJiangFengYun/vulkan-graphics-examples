#include "graphics/mesh/mesh_data.hpp"

namespace kgs
{
	uint32_t MeshData::getDataBaseTypeSize(const DataType dataType)
	{
		switch (dataType)
		{
		case DataType::FLOAT:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::FLOAT>::BaseType));
			break;
		}
		case DataType::FLOAT_ARRAY:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::FLOAT_ARRAY>::BaseType));
			break;
		}
		case DataType::INT:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::INT>::BaseType));
			break;
		}
		case DataType::INT_ARRAY:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::INT_ARRAY>::BaseType));
			break;
		}
		case DataType::VECTOR_2:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_2>::BaseType));
			break;
		}
		case DataType::VECTOR_2_ARRAY:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_2_ARRAY>::BaseType));
			break;
		}
		case DataType::VECTOR_3:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_3>::BaseType));
			break;
		}
		case DataType::VECTOR_3_ARRAY:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_3_ARRAY>::BaseType));
			break;
		}
		case DataType::VECTOR_4:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_4>::BaseType));
			break;
		}
		case DataType::VECTOR_4_ARRAY:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_4_ARRAY>::BaseType));
			break;
		}
		case DataType::COLOR_32:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::COLOR_32>::BaseType));
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
		case DataType::FLOAT:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::FLOAT>::BaseType));
			break;
		}
		case DataType::FLOAT_ARRAY:
		{
			auto arr = getValue(name, mapFloatArrays, KGS_TRUE);
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::FLOAT_ARRAY>::BaseType) * arr.size());
			break;
		}
		case DataType::INT:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::INT>::BaseType));
			break;
		}
		case DataType::INT_ARRAY:
		{
			auto arr = getValue(name, mapIntArrays, KGS_TRUE);
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::INT_ARRAY>::BaseType) * arr.size());
			break;
		}
		case DataType::VECTOR_2:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_2>::BaseType));
			break;
		}
		case DataType::VECTOR_2_ARRAY:
		{
			auto arr = getValue(name, mapVector2Arrays, KGS_TRUE);
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_2_ARRAY>::BaseType) * arr.size());
			break;
		}
		case DataType::VECTOR_3:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_3>::BaseType));
			break;
		}
		case DataType::VECTOR_3_ARRAY:
		{
			auto arr = getValue(name, mapVector3Arrays, KGS_TRUE);
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_3_ARRAY>::BaseType) * arr.size());
			break;
		}
		case DataType::VECTOR_4:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_4>::BaseType));
			break;
		}
		case DataType::VECTOR_4_ARRAY:
		{
			auto arr = getValue(name, mapVector4Arrays, KGS_TRUE);
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_4_ARRAY>::BaseType) * arr.size());
			break;
		}
		case DataType::COLOR_32:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::COLOR_32>::BaseType));
			break;
		}
		case DataType::COLOR_32_ARRAY:
		{
			auto arr = getValue(name, mapColor32Arrays, KGS_TRUE);
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
		case DataType::FLOAT:
		{
			auto value = getValue(name, mapFloats, KGS_TRUE);
			std::memcpy(ptr, &value, sizeof(DataTypeInfo<DataType::FLOAT>::BaseType));
			break;
		}
		case DataType::FLOAT_ARRAY:
		{
			auto arr = getValue(name, mapFloatArrays, KGS_TRUE);
			std::memcpy(ptr, arr.data() + elementStart, 
				sizeof(DataTypeInfo<DataType::FLOAT_ARRAY>::BaseType) * std::min(static_cast<uint32_t>(arr.size()) - elementStart, maxElementCount));
			break;
		}
		case DataType::INT:
		{
			auto value = getValue(name, mapInts, KGS_TRUE);
			std::memcpy(ptr, &value, sizeof(DataTypeInfo<DataType::INT>::BaseType));
			break;
		}
		case DataType::INT_ARRAY:
		{
			auto arr = getValue(name, mapIntArrays, KGS_TRUE);
			std::memcpy(ptr, arr.data() + elementStart,
				sizeof(DataTypeInfo<DataType::INT_ARRAY>::BaseType) * std::min(static_cast<uint32_t>(arr.size()) - elementStart, maxElementCount));
			break;
		}
		case DataType::VECTOR_2:
		{
			auto value = getValue(name, mapVector2s, KGS_TRUE);
			std::memcpy(ptr, &value, sizeof(DataTypeInfo<DataType::VECTOR_2>::BaseType));
			break;
		}
		case DataType::VECTOR_2_ARRAY:
		{
			auto arr = getValue(name, mapVector2Arrays, KGS_TRUE);
			std::memcpy(ptr, arr.data() + elementStart, 
				sizeof(DataTypeInfo<DataType::VECTOR_2_ARRAY>::BaseType) * std::min(static_cast<uint32_t>(arr.size()) - elementStart, maxElementCount));
			break;
		}
		case DataType::VECTOR_3:
		{
			auto value = getValue(name, mapVector3s, KGS_TRUE);
			std::memcpy(ptr, &value, sizeof(DataTypeInfo<DataType::VECTOR_3>::BaseType));
			break;
		}
		case DataType::VECTOR_3_ARRAY:
		{
			auto arr = getValue(name, mapVector3Arrays, KGS_TRUE);
			std::memcpy(ptr, arr.data() + elementStart, 
				sizeof(DataTypeInfo<DataType::VECTOR_3_ARRAY>::BaseType) * std::min(static_cast<uint32_t>(arr.size()) - elementStart, maxElementCount));
			break;
		}
		case DataType::VECTOR_4:
		{
			auto value = getValue(name, mapVector4s, KGS_TRUE);
			std::memcpy(ptr, &value, sizeof(DataTypeInfo<DataType::VECTOR_4>::BaseType));
			break;
		}
		case DataType::VECTOR_4_ARRAY:
		{
			auto arr = getValue(name, mapVector4Arrays, KGS_TRUE);
			std::memcpy(ptr, arr.data() + elementStart, 
				sizeof(DataTypeInfo<DataType::VECTOR_4_ARRAY>::BaseType) * std::min(static_cast<uint32_t>(arr.size()) - elementStart, maxElementCount));
			break;
		}
		case DataType::COLOR_32:
		{
			auto value = getValue(name, mapColor32s, KGS_TRUE);
			std::memcpy(ptr, &value, sizeof(DataTypeInfo<DataType::COLOR_32>::BaseType));
			break;
		}
		case DataType::COLOR_32_ARRAY:
		{
			auto arr = getValue(name, mapColor32Arrays, KGS_TRUE);
			std::memcpy(ptr, arr.data() + elementStart, 
				sizeof(DataTypeInfo<DataType::COLOR_32_ARRAY>::BaseType) * std::min(static_cast<uint32_t>(arr.size()) - elementStart, maxElementCount));
			break;
		}
		default:
			throw std::runtime_error("Invalid data type for copying its data to memory.");
		}
	}

	//------------------this used to eliminate ide warning------------------------------
	template <MeshData::DataType type>
	typename MeshData::DataTypeInfo<type>::ValueType MeshData::getDataValue(std::string name) { return {}; }

	template <MeshData::DataType type>
	void MeshData::setDataValue(std::string name, typename MeshData::DataTypeInfo<type>::ValueType value) {}
	//-----------------this used to eliminate ide warning----------------

	template<>
	float MeshData::getDataValue<MeshData::DataType::FLOAT>(std::string name)
	{
		return getValue(name, mapFloats);
	}

	template<>
	void MeshData::setDataValue<MeshData::DataType::FLOAT>(std::string name, float value)
	{
		setValue(name, value, mapFloats);
	}

	template<>
	std::vector<float> MeshData::getDataValue<MeshData::DataType::FLOAT_ARRAY>(std::string name)
	{
		return getValue(name, mapFloatArrays);
	}

	template<>
	void MeshData::setDataValue<MeshData::DataType::FLOAT_ARRAY>(std::string name, std::vector<float> value)
	{
		setValue(name, value, mapFloatArrays);
	}

	template<>
	int32_t MeshData::getDataValue<MeshData::DataType::INT>(std::string name)
	{
		return getValue(name, mapInts);
	}

	template<>
	void MeshData::setDataValue<MeshData::DataType::INT>(std::string name, int32_t value)
	{
		setValue(name, value, mapInts);
	}

	template<>
	std::vector<int32_t> MeshData::getDataValue<MeshData::DataType::INT_ARRAY>(std::string name)
	{
		return getValue(name, mapIntArrays);
	}

	template<>
	void MeshData::setDataValue<MeshData::DataType::INT_ARRAY>(std::string name, std::vector<int32_t> value)
	{
		setValue(name, value, mapIntArrays);
	}

	template<>
	Vector2 MeshData::getDataValue<MeshData::DataType::VECTOR_2>(std::string name)
	{
		return getValue(name, mapVector2s);
	}

	template<>
	void MeshData::setDataValue<MeshData::DataType::VECTOR_2>(std::string name, Vector2 value)
	{
		setValue(name, value, mapVector2s);
	}

	template<>
	std::vector<Vector2> MeshData::getDataValue<MeshData::DataType::VECTOR_2_ARRAY>(std::string name)
	{
		return getValue(name, mapVector2Arrays);
	}

	template<>
	void MeshData::setDataValue<MeshData::DataType::VECTOR_2_ARRAY>(std::string name, std::vector<Vector2> value)
	{
		setValue(name, value, mapVector2Arrays);
	}

	template<>
	Vector3 MeshData::getDataValue<MeshData::DataType::VECTOR_3>(std::string name)
	{
		return getValue(name, mapVector3s);
	}

	template<>
	void MeshData::setDataValue<MeshData::DataType::VECTOR_3>(std::string name, Vector3 value)
	{
		setValue(name, value, mapVector3s);
	}

	template<>
	std::vector<Vector3> MeshData::getDataValue<MeshData::DataType::VECTOR_3_ARRAY>(std::string name)
	{
		return getValue(name, mapVector3Arrays);
	}

	template<>
	void MeshData::setDataValue<MeshData::DataType::VECTOR_3_ARRAY>(std::string name, std::vector<Vector3> value)
	{
		setValue(name, value, mapVector3Arrays);
	}

	template<>
	Vector4 MeshData::getDataValue<MeshData::DataType::VECTOR_4>(std::string name)
	{
		return getValue(name, mapVector4s);
	}

	template<>
	void MeshData::setDataValue<MeshData::DataType::VECTOR_4>(std::string name, Vector4 value)
	{
		setValue(name, value, mapVector4s);
	}

	template<>
	std::vector<Vector4> MeshData::getDataValue<MeshData::DataType::VECTOR_4_ARRAY>(std::string name)
	{
		return getValue(name, mapVector4Arrays);
	}

	template<>
	void MeshData::setDataValue<MeshData::DataType::VECTOR_4_ARRAY>(std::string name, std::vector<Vector4> value)
	{
		setValue(name, value, mapVector4Arrays);
	}

	template<>
	Color32 MeshData::getDataValue<MeshData::DataType::COLOR_32>(std::string name)
	{
		return getValue(name, mapColor32s);
	}

	template<>
	void MeshData::setDataValue<MeshData::DataType::COLOR_32>(std::string name, Color32 value)
	{
		setValue(name, value, mapColor32s);
	}

	template<>
	std::vector<Color32> MeshData::getDataValue<MeshData::DataType::COLOR_32_ARRAY>(std::string name)
	{
		return getValue(name, mapColor32Arrays);
	}

	template<>
	void MeshData::setDataValue<MeshData::DataType::COLOR_32_ARRAY>(std::string name, std::vector<Color32> value)
	{
		setValue(name, value, mapColor32Arrays);
	}
}