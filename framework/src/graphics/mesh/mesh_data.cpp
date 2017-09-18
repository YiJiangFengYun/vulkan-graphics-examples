#include "graphics/mesh/mesh_data.hpp"

namespace kgs
{
	uint32_t MeshData::getDataBaseTypeSize(const DataType dataType)
	{
		switch (dataType)
		{
		case DataType::FLOAT:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::FLOAT>::base_t));
			break;
		}
		case DataType::FLOAT_ARRAY:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::FLOAT_ARRAY>::base_t));
			break;
		}
		case DataType::INT:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::INT>::base_t));
			break;
		}
		case DataType::INT_ARRAY:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::INT_ARRAY>::base_t));
			break;
		}
		case DataType::VECTOR_2:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_2>::base_t));
			break;
		}
		case DataType::VECTOR_2_ARRAY:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_2_ARRAY>::base_t));
			break;
		}
		case DataType::VECTOR_3:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_3>::base_t));
			break;
		}
		case DataType::VECTOR_3_ARRAY:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_3_ARRAY>::base_t));
			break;
		}
		case DataType::VECTOR_4:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_4>::base_t));
			break;
		}
		case DataType::VECTOR_4_ARRAY:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_4_ARRAY>::base_t));
			break;
		}
		case DataType::COLOR_32:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::COLOR_32>::base_t));
			break;
		}
		case DataType::COLOR_32_ARRAY:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::COLOR_32_ARRAY>::base_t));
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
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::FLOAT>::base_t));
			break;
		}
		case DataType::FLOAT_ARRAY:
		{
			auto arr = getValue(name, mapFloatArrays, arrFloatArrays, KGS_TRUE);
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::FLOAT_ARRAY>::base_t) * arr.size());
			break;
		}
		case DataType::INT:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::INT>::base_t));
			break;
		}
		case DataType::INT_ARRAY:
		{
			auto arr = getValue(name, mapIntArrays, arrIntArrays, KGS_TRUE);
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::INT_ARRAY>::base_t) * arr.size());
			break;
		}
		case DataType::VECTOR_2:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_2>::base_t));
			break;
		}
		case DataType::VECTOR_2_ARRAY:
		{
			auto arr = getValue(name, mapVector2Arrays, arrVector2Arrays, KGS_TRUE);
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_2_ARRAY>::base_t) * arr.size());
			break;
		}
		case DataType::VECTOR_3:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_3>::base_t));
			break;
		}
		case DataType::VECTOR_3_ARRAY:
		{
			auto arr = getValue(name, mapVector3Arrays, arrVector3Arrays, KGS_TRUE);
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_3_ARRAY>::base_t) * arr.size());
			break;
		}
		case DataType::VECTOR_4:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_4>::base_t));
			break;
		}
		case DataType::VECTOR_4_ARRAY:
		{
			auto arr = getValue(name, mapVector4Arrays, arrVector4Arrays, KGS_TRUE);
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_4_ARRAY>::base_t) * arr.size());
			break;
		}
		case DataType::COLOR_32:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::COLOR_32>::base_t));
			break;
		}
		case DataType::COLOR_32_ARRAY:
		{
			auto arr = getValue(name, mapColor32Arrays, arrColor32Arrays, KGS_TRUE);
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::COLOR_32_ARRAY>::base_t) * arr.size());
			break;
		}
		default:
			throw std::runtime_error("Invalid data type for getting its used memory size.");
		}
	}

	void MeshData::memCopyDataValue(std::string name, DataType dataType, void* dst, uint32_t offset, uint32_t maxElementCount)
	{
		char *ptr = static_cast<char *>(dst);
		ptr += offset;
		switch (dataType)
		{
		case DataType::FLOAT:
		{
			auto value = getValue(name, mapFloats, arrFloats, KGS_TRUE);
			std::memcpy(ptr, &value, sizeof(DataTypeInfo<DataType::FLOAT>::base_t));
			break;
		}
		case DataType::FLOAT_ARRAY:
		{
			auto arr = getValue(name, mapFloatArrays, arrFloatArrays, KGS_TRUE);
			std::memcpy(ptr, arr.data(), sizeof(DataTypeInfo<DataType::FLOAT_ARRAY>::base_t) * std::min(static_cast<uint32_t>(arr.size()), maxElementCount));
			break;
		}
		case DataType::INT:
		{
			auto value = getValue(name, mapInts, arrInts, KGS_TRUE);
			std::memcpy(ptr, &value, sizeof(DataTypeInfo<DataType::INT>::base_t));
			break;
		}
		case DataType::INT_ARRAY:
		{
			auto arr = getValue(name, mapIntArrays, arrIntArrays, KGS_TRUE);
			std::memcpy(ptr, arr.data(), sizeof(DataTypeInfo<DataType::INT_ARRAY>::base_t) * std::min(static_cast<uint32_t>(arr.size()), maxElementCount));
			break;
		}
		case DataType::VECTOR_2:
		{
			auto value = getValue(name, mapVector2s, arrVector2s, KGS_TRUE);
			std::memcpy(ptr, &value, sizeof(DataTypeInfo<DataType::VECTOR_2>::base_t));
			break;
		}
		case DataType::VECTOR_2_ARRAY:
		{
			auto arr = getValue(name, mapVector2Arrays, arrVector2Arrays, KGS_TRUE);
			std::memcpy(ptr, arr.data(), sizeof(DataTypeInfo<DataType::VECTOR_2_ARRAY>::base_t) * std::min(static_cast<uint32_t>(arr.size()), maxElementCount));
			break;
		}
		case DataType::VECTOR_3:
		{
			auto value = getValue(name, mapVector3s, arrVector3s, KGS_TRUE);
			std::memcpy(ptr, &value, sizeof(DataTypeInfo<DataType::VECTOR_3>::base_t));
			break;
		}
		case DataType::VECTOR_3_ARRAY:
		{
			auto arr = getValue(name, mapVector3Arrays, arrVector3Arrays, KGS_TRUE);
			std::memcpy(ptr, arr.data(), sizeof(DataTypeInfo<DataType::VECTOR_3_ARRAY>::base_t) * std::min(static_cast<uint32_t>(arr.size()), maxElementCount));
			break;
		}
		case DataType::VECTOR_4:
		{
			auto value = getValue(name, mapVector4s, arrVector4s, KGS_TRUE);
			std::memcpy(ptr, &value, sizeof(DataTypeInfo<DataType::VECTOR_4>::base_t));
			break;
		}
		case DataType::VECTOR_4_ARRAY:
		{
			auto arr = getValue(name, mapVector4Arrays, arrVector4Arrays, KGS_TRUE);
			std::memcpy(ptr, arr.data(), sizeof(DataTypeInfo<DataType::VECTOR_4_ARRAY>::base_t) * std::min(static_cast<uint32_t>(arr.size()), maxElementCount));
			break;
		}
		case DataType::COLOR_32:
		{
			auto value = getValue(name, mapColor32s, arrColor32s, KGS_TRUE);
			std::memcpy(ptr, &value, sizeof(DataTypeInfo<DataType::COLOR_32>::base_t));
			break;
		}
		case DataType::COLOR_32_ARRAY:
		{
			auto arr = getValue(name, mapColor32Arrays, arrColor32Arrays, KGS_TRUE);
			std::memcpy(ptr, arr.data(), sizeof(DataTypeInfo<DataType::COLOR_32_ARRAY>::base_t) * std::min(static_cast<uint32_t>(arr.size()), maxElementCount));
			break;
		}
		default:
			throw std::runtime_error("Invalid data type for copying its data to memory.");
		}
	}

	//------------------this used to eliminate ide warning------------------------------
	template <MeshData::DataType type>
	typename MeshData::DataType<type>::value_t MeshData::getDataValue(std::string name) { return {}; }

	template <MeshData::DataType type>
	void MeshData::setDataValue(std::string name, typename MeshData::DataType<type>::value_t value) {}
	//-----------------this used to eliminate ide warning----------------

	template<>
	float MeshData::getDataValue<MeshData::DataType::FLOAT>(std::string name)
	{
		return getValue(name, mapFloats, arrFloats);
	}

	template<>
	void MeshData::setDataValue<MeshData::DataType::FLOAT>(std::string name, float value)
	{
		setValue(name, value, mapFloats, arrFloats);
	}

	template<>
	std::vector<float> MeshData::getDataValue<MeshData::DataType::FLOAT_ARRAY>(std::string name)
	{
		return getValue(name, mapFloatArrays, arrFloatArrays);
	}

	template<>
	void MeshData::setDataValue<MeshData::DataType::FLOAT_ARRAY>(std::string name, std::vector<float> value)
	{
		setValue(name, value, mapFloatArrays, arrFloatArrays);
	}

	template<>
	int32_t MeshData::getDataValue<MeshData::DataType::INT>(std::string name)
	{
		return getValue(name, mapInts, arrInts);
	}

	template<>
	void MeshData::setDataValue<MeshData::DataType::INT>(std::string name, int32_t value)
	{
		setValue(name, value, mapInts, arrInts);
	}

	template<>
	std::vector<int32_t> MeshData::getDataValue<MeshData::DataType::INT_ARRAY>(std::string name)
	{
		return getValue(name, mapIntArrays, arrIntArrays);
	}

	template<>
	void MeshData::setDataValue<MeshData::DataType::INT_ARRAY>(std::string name, std::vector<int32_t> value)
	{
		setValue(name, value, mapIntArrays, arrIntArrays);
	}

	template<>
	Vector2 MeshData::getDataValue<MeshData::DataType::VECTOR_2>(std::string name)
	{
		return getValue(name, mapVector2s, arrVector2s);
	}

	template<>
	void MeshData::setDataValue<MeshData::DataType::VECTOR_2>(std::string name, Vector2 value)
	{
		setValue(name, value, mapVector2s, arrVector2s);
	}

	template<>
	std::vector<Vector2> MeshData::getDataValue<MeshData::DataType::VECTOR_2_ARRAY>(std::string name)
	{
		return getValue(name, mapVector2Arrays, arrVector2Arrays);
	}

	template<>
	void MeshData::setDataValue<MeshData::DataType::VECTOR_2_ARRAY>(std::string name, std::vector<Vector2> value)
	{
		setValue(name, value, mapVector2Arrays, arrVector2Arrays);
	}

	template<>
	Vector3 MeshData::getDataValue<MeshData::DataType::VECTOR_3>(std::string name)
	{
		return getValue(name, mapVector3s, arrVector3s);
	}

	template<>
	void MeshData::setDataValue<MeshData::DataType::VECTOR_3>(std::string name, Vector3 value)
	{
		setValue(name, value, mapVector3s, arrVector3s);
	}

	template<>
	std::vector<Vector3> MeshData::getDataValue<MeshData::DataType::VECTOR_3_ARRAY>(std::string name)
	{
		return getValue(name, mapVector3Arrays, arrVector3Arrays);
	}

	template<>
	void MeshData::setDataValue<MeshData::DataType::VECTOR_3_ARRAY>(std::string name, std::vector<Vector3> value)
	{
		setValue(name, value, mapVector3Arrays, arrVector3Arrays);
	}

	template<>
	Vector4 MeshData::getDataValue<MeshData::DataType::VECTOR_4>(std::string name)
	{
		return getValue(name, mapVector4s, arrVector4s);
	}

	template<>
	void MeshData::setDataValue<MeshData::DataType::VECTOR_4>(std::string name, Vector4 value)
	{
		setValue(name, value, mapVector4s, arrVector4s);
	}

	template<>
	std::vector<Vector4> MeshData::getDataValue<MeshData::DataType::VECTOR_4_ARRAY>(std::string name)
	{
		return getValue(name, mapVector4Arrays, arrVector4Arrays);
	}

	template<>
	void MeshData::setDataValue<MeshData::DataType::VECTOR_4_ARRAY>(std::string name, std::vector<Vector4> value)
	{
		setValue(name, value, mapVector4Arrays, arrVector4Arrays);
	}

	template<>
	Color32 MeshData::getDataValue<MeshData::DataType::COLOR_32>(std::string name)
	{
		return getValue(name, mapColor32s, arrColor32s);
	}

	template<>
	void MeshData::setDataValue<MeshData::DataType::COLOR_32>(std::string name, Color32 value)
	{
		setValue(name, value, mapColor32s, arrColor32s);
	}

	template<>
	std::vector<Color32> MeshData::getDataValue<MeshData::DataType::COLOR_32_ARRAY>(std::string name)
	{
		return getValue(name, mapColor32Arrays, arrColor32Arrays);
	}

	template<>
	void MeshData::setDataValue<MeshData::DataType::COLOR_32_ARRAY>(std::string name, std::vector<Color32> value)
	{
		setValue(name, value, mapColor32Arrays, arrColor32Arrays);
	}
}