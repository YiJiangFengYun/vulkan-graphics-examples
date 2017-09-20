#include "graphics/material/material_data.hpp"

namespace kgs
{
	uint32_t MaterialData::getDataBaseTypeSize(DataType dataType)
	{
		switch (dataType)
		{
		case DataType::FLOAT:
		case DataType::FLOAT_ARRAY:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::FLOAT>::BaseType));
		}
		case DataType::INT:
		case DataType::INT_ARRAY:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::INT>::BaseType));
		}
		case DataType::COLOR:
		case DataType::COLOR_ARRAY:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::COLOR>::BaseType));
		}
		case DataType::VECTOR:
		case DataType::VECTOR_ARRAY:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR>::BaseType));
		}
		case DataType::MATRIX:
		case DataType::MATRIX_ARRAY:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::MATRIX>::BaseType));
		}
		case DataType::TEXTURE_OFFSET:
		case DataType::TEXTURE_SCALE:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::TEXTURE_OFFSET>::BaseType));
		}
		default:
			throw std::runtime_error("Invalid data type for getting memeory size used by its base type.");
		}
	}

	uint32_t MaterialData::getDataValueSize(std::string name, DataType dataType)
	{
		switch (dataType)
		{
		case DataType::FLOAT:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::FLOAT>::BaseType));
		}
		case DataType::FLOAT_ARRAY:
		{
			auto arr = getValue(name, mapFloatArrays, arrFloatArrays, KGS_TRUE);
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::FLOAT_ARRAY>::BaseType) * arr.size());
		}
		case DataType::INT:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::INT>::BaseType));
		}
		case DataType::INT_ARRAY:
		{
			auto arr = getValue(name, mapIntArrays, arrIntArrays, KGS_TRUE);
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::INT_ARRAY>::BaseType) * arr.size());
		}
		case DataType::COLOR:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::COLOR>::BaseType));
		}
		case DataType::COLOR_ARRAY:
		{
			auto arr = getValue(name, mapColorArrays, arrColorArrays, KGS_TRUE);
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::COLOR_ARRAY>::BaseType) * arr.size());
		}
		case DataType::VECTOR:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR>::BaseType));
		}
		case DataType::VECTOR_ARRAY:
		{
			auto arr = getValue(name, mapVectorArrays, arrVectorArrays, KGS_TRUE);
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_ARRAY>::BaseType) * arr.size());
		}
		case DataType::MATRIX:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::MATRIX>::BaseType));
		}
		case DataType::MATRIX_ARRAY:
		{
			auto arr = getValue(name, mapMatrixArrays, arrMatrixArrays, KGS_TRUE);
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::MATRIX_ARRAY>::BaseType) * arr.size());
		}
		case DataType::TEXTURE_OFFSET:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::TEXTURE_OFFSET>::BaseType));
		}
		case DataType::TEXTURE_SCALE:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::TEXTURE_SCALE>::BaseType));
		}
		default:
			throw std::runtime_error("Invalid data type for getting its used memory size.");
		}
	}

	void MaterialData::memCopyDataValue(std::string name, DataType dataType, void* dst, uint32_t offset, uint32_t elementStart, uint32_t maxElementCount)
	{
		char *ptr = static_cast<char *>(dst);
		ptr += offset;
		switch (dataType)
		{
		case DataType::FLOAT:
		{
			auto value = getValue(name, mapFloats, arrFloats, KGS_TRUE);
			std::memcpy(ptr, &value, sizeof(DataTypeInfo<DataType::FLOAT>::BaseType));
		}
		case DataType::FLOAT_ARRAY:
		{
			auto arr = getValue(name, mapFloatArrays, arrFloatArrays, KGS_TRUE);
			uint32_t finalElementCount = std::max(0u, std::min(static_cast<uint32_t>(arr.size()) - elementStart, maxElementCount));
			if (finalElementCount == 0) break;
			std::memcpy(ptr, arr.data() + elementStart, sizeof(DataTypeInfo<DataType::FLOAT_ARRAY>::BaseType) * finalElementCount);
		}
		case DataType::INT:
		{
			auto value = getValue(name, mapInts, arrInts, KGS_TRUE);
			std::memcpy(ptr, &value, sizeof(DataTypeInfo<DataType::INT>::BaseType));
		}
		case DataType::INT_ARRAY:
		{
			auto arr = getValue(name, mapIntArrays, arrIntArrays, KGS_TRUE);
			uint32_t finalElementCount = std::max(0u, std::min(static_cast<uint32_t>(arr.size()) - elementStart, maxElementCount));
			if (finalElementCount == 0) break;
			std::memcpy(ptr, arr.data() + elementStart, sizeof(DataTypeInfo<DataType::INT_ARRAY>::BaseType) * finalElementCount);
		}
		case DataType::COLOR:
		{
			auto value = getValue(name, mapColors, arrColors, KGS_TRUE);
			std::memcpy(ptr, &value, sizeof(DataTypeInfo<DataType::COLOR>::BaseType));
		}
		case DataType::COLOR_ARRAY:
		{
			auto arr = getValue(name, mapColorArrays, arrColorArrays, KGS_TRUE);
			uint32_t finalElementCount = std::max(0u, std::min(static_cast<uint32_t>(arr.size()) - elementStart, maxElementCount));
			if (finalElementCount == 0) break;
			std::memcpy(ptr, arr.data() + elementStart, sizeof(DataTypeInfo<DataType::COLOR_ARRAY>::BaseType) * finalElementCount);
		}
		case DataType::VECTOR:
		{
			auto value = getValue(name, mapVectors, arrVectors, KGS_TRUE);
			std::memcpy(ptr, &value, sizeof(DataTypeInfo<DataType::VECTOR>::BaseType));
		}
		case DataType::VECTOR_ARRAY:
		{
			auto arr = getValue(name, mapVectorArrays, arrVectorArrays, KGS_TRUE);
			uint32_t finalElementCount = std::max(0u, std::min(static_cast<uint32_t>(arr.size()) - elementStart, maxElementCount));
			if (finalElementCount == 0) break;
			std::memcpy(ptr, arr.data() + elementStart, sizeof(DataTypeInfo<DataType::VECTOR_ARRAY>::BaseType) * finalElementCount);
		}
		case DataType::MATRIX:
		{
			auto value = getValue(name, mapMatrixs, arrMatrixs, KGS_TRUE);
			std::memcpy(ptr, &value, sizeof(DataTypeInfo<DataType::MATRIX>::BaseType));
		}
		case DataType::MATRIX_ARRAY:
		{
			auto arr = getValue(name, mapMatrixArrays, arrMatrixArrays, KGS_TRUE);
			uint32_t finalElementCount = std::max(0u, std::min(static_cast<uint32_t>(arr.size()) - elementStart, maxElementCount));
			if (finalElementCount == 0) break;
			std::memcpy(ptr, arr.data() + elementStart, sizeof(DataTypeInfo<DataType::MATRIX_ARRAY>::BaseType) * finalElementCount);
		}
		case DataType::TEXTURE_OFFSET:
		{
			auto value = getValue(name, mapTextureOffsets, arrTextureOffsets, KGS_TRUE);
			std::memcpy(ptr, &value, sizeof(DataTypeInfo<DataType::TEXTURE_OFFSET>::BaseType));
		}
		case DataType::TEXTURE_SCALE:
		{
			auto value = getValue(name, mapTextureScales, arrTextureScales, KGS_TRUE);
			std::memcpy(ptr, &value, sizeof(DataTypeInfo<DataType::TEXTURE_SCALE>::BaseType));
		}
		default:
			throw std::runtime_error("Invalid data type for copying its data to memory.");
		}
	}

	//-----------------this used to eliminate ide warning----------------
	template <MaterialData::DataType type>
	typename MaterialData::DataTypeInfo<type>::ValueType MaterialData::getDataValue(std::string name) { return {}; }

	template<MaterialData::DataType type>
	void MaterialData::setDataValue(std::string name, typename MaterialData::DataTypeInfo<type>::ValueType value) {}
	//-----------------this used to eliminate ide warning----------------

	template<>
	float MaterialData::getDataValue<MaterialData::DataType::FLOAT>(std::string name)
	{
		return getValue(name, mapFloats, arrFloats);
	}

	template<>
	void MaterialData::setDataValue<MaterialData::DataType::FLOAT>(std::string name, float value)
	{
		setValue(name, value, mapFloats, arrFloats);
	}

	template<>
	std::vector<float> MaterialData::getDataValue<MaterialData::DataType::FLOAT_ARRAY>(std::string name)
	{
		return getValue(name, mapFloatArrays, arrFloatArrays);
	}

	template<>
	void MaterialData::setDataValue<MaterialData::DataType::FLOAT_ARRAY>(std::string name, std::vector<float> value)
	{
		setValue(name, value, mapFloatArrays, arrFloatArrays);
	}

	template<>
	int32_t MaterialData::getDataValue<MaterialData::DataType::INT>(std::string name)
	{
		return getValue(name, mapInts, arrInts);
	}

	template<>
	void MaterialData::setDataValue<MaterialData::DataType::INT>(std::string name, int32_t value)
	{
		setValue(name, value, mapInts, arrInts);
	}

	template<>
	std::vector<int32_t> MaterialData::getDataValue<MaterialData::DataType::INT_ARRAY>(std::string name)
	{
		return getValue(name, mapIntArrays, arrIntArrays);
	}

	template<>
	void MaterialData::setDataValue<MaterialData::DataType::INT_ARRAY>(std::string name, std::vector<int32_t> value)
	{
		setValue(name, value, mapIntArrays, arrIntArrays);
	}

	template<>
	Color MaterialData::getDataValue<MaterialData::DataType::COLOR>(std::string name)
	{
		return getValue(name, mapColors, arrColors);
	}

	template<>
	void MaterialData::setDataValue<MaterialData::DataType::COLOR>(std::string name, Color value)
	{
		setValue(name, value, mapColors, arrColors);
	}

	template<>
	std::vector<Color> MaterialData::getDataValue<MaterialData::DataType::COLOR_ARRAY>(std::string name)
	{
		return getValue(name, mapColorArrays, arrColorArrays);
	}

	template<>
	void MaterialData::setDataValue<MaterialData::DataType::COLOR_ARRAY>(std::string name, std::vector<Color> value)
	{
		setValue(name, value, mapColorArrays, arrColorArrays);
	}

	template<>
	Vector4 MaterialData::getDataValue<MaterialData::DataType::VECTOR>(std::string name)
	{
		return getValue(name, mapVectors, arrVectors);
	}

	template<>
	void MaterialData::setDataValue<MaterialData::DataType::VECTOR>(std::string name, Vector4 value)
	{
		setValue(name, value, mapVectors, arrVectors);
	}

	template<>
	std::vector<Vector4> MaterialData::getDataValue<MaterialData::DataType::VECTOR_ARRAY>(std::string name)
	{
		return getValue(name, mapVectorArrays, arrVectorArrays);
	}

	template<>
	void MaterialData::setDataValue<MaterialData::DataType::VECTOR_ARRAY>(std::string name, std::vector<Vector4> value)
	{
		setValue(name, value, mapVectorArrays, arrVectorArrays);
	}

	template<>
	Matrix4x4 MaterialData::getDataValue<MaterialData::DataType::MATRIX>(std::string name)
	{
		return getValue(name, mapMatrixs, arrMatrixs);
	}

	template<>
	void MaterialData::setDataValue<MaterialData::DataType::MATRIX>(std::string name, Matrix4x4 value)
	{
		setValue(name, value, mapMatrixs, arrMatrixs);
	}

	template<>
	std::vector<Matrix4x4> MaterialData::getDataValue<MaterialData::DataType::MATRIX_ARRAY>(std::string name)
	{
		return getValue(name, mapMatrixArrays, arrMatrixArrays);
	}

	template<>
	void MaterialData::setDataValue<MaterialData::DataType::MATRIX_ARRAY>(std::string name, std::vector<Matrix4x4> value)
	{
		setValue(name, value, mapMatrixArrays, arrMatrixArrays);
	}

	template<>
	std::shared_ptr<Texture> MaterialData::getDataValue<MaterialData::DataType::TEXTURE>(std::string name)
	{
		return getValue(name, mapTextures, arrTextures);
	}

	template<>
	void MaterialData::setDataValue<MaterialData::DataType::TEXTURE>(std::string name, std::shared_ptr<Texture> value)
	{
		setValue(name, value, mapTextures, arrTextures);
	}

	template<>
	Vector2 MaterialData::getDataValue<MaterialData::DataType::TEXTURE_OFFSET>(std::string name)
	{
		return getValue(name, mapTextureOffsets, arrTextureOffsets);
	}

	template<>
	void MaterialData::setDataValue<MaterialData::DataType::TEXTURE_OFFSET>(std::string name, Vector2 value)
	{
		setValue(name, value, mapTextureOffsets, arrTextureOffsets);
	}

	template<>
	Vector2 MaterialData::getDataValue<MaterialData::DataType::TEXTURE_SCALE>(std::string name)
	{
		return getValue(name, mapTextureScales, arrTextureScales);
	}

	template<>
	void MaterialData::setDataValue<MaterialData::DataType::TEXTURE_SCALE>(std::string name, Vector2 value)
	{
		setValue(name, value, mapTextureScales, arrTextureScales);
	}
}

