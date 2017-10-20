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
			auto arr = getValue(name, mapFloatArrays);
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::FLOAT_ARRAY>::BaseType) * arr.size());
		}
		case DataType::INT:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::INT>::BaseType));
		}
		case DataType::INT_ARRAY:
		{
			auto arr = getValue(name, mapIntArrays);
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::INT_ARRAY>::BaseType) * arr.size());
		}
		case DataType::COLOR:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::COLOR>::BaseType));
		}
		case DataType::COLOR_ARRAY:
		{
			auto arr = getValue(name, mapColorArrays);
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::COLOR_ARRAY>::BaseType) * arr.size());
		}
		case DataType::VECTOR:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR>::BaseType));
		}
		case DataType::VECTOR_ARRAY:
		{
			auto arr = getValue(name, mapVectorArrays);
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::VECTOR_ARRAY>::BaseType) * arr.size());
		}
		case DataType::MATRIX:
		{
			return static_cast<uint32_t>(sizeof(DataTypeInfo<DataType::MATRIX>::BaseType));
		}
		case DataType::MATRIX_ARRAY:
		{
			auto arr = getValue(name, mapMatrixArrays);
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
			auto value = getValue(name, mapFloats);
			std::memcpy(ptr, &value, sizeof(DataTypeInfo<DataType::FLOAT>::BaseType));
		}
		case DataType::FLOAT_ARRAY:
		{
			auto arr = getValue(name, mapFloatArrays);
			uint32_t finalElementCount = std::max(0u, std::min(static_cast<uint32_t>(arr.size()) - elementStart, maxElementCount));
			if (finalElementCount == 0) break;
			std::memcpy(ptr, arr.data() + elementStart, sizeof(DataTypeInfo<DataType::FLOAT_ARRAY>::BaseType) * finalElementCount);
		}
		case DataType::INT:
		{
			auto value = getValue(name, mapInts);
			std::memcpy(ptr, &value, sizeof(DataTypeInfo<DataType::INT>::BaseType));
		}
		case DataType::INT_ARRAY:
		{
			auto arr = getValue(name, mapIntArrays);
			uint32_t finalElementCount = std::max(0u, std::min(static_cast<uint32_t>(arr.size()) - elementStart, maxElementCount));
			if (finalElementCount == 0) break;
			std::memcpy(ptr, arr.data() + elementStart, sizeof(DataTypeInfo<DataType::INT_ARRAY>::BaseType) * finalElementCount);
		}
		case DataType::COLOR:
		{
			auto value = getValue(name, mapColors);
			std::memcpy(ptr, &value, sizeof(DataTypeInfo<DataType::COLOR>::BaseType));
		}
		case DataType::COLOR_ARRAY:
		{
			auto arr = getValue(name, mapColorArrays);
			uint32_t finalElementCount = std::max(0u, std::min(static_cast<uint32_t>(arr.size()) - elementStart, maxElementCount));
			if (finalElementCount == 0) break;
			std::memcpy(ptr, arr.data() + elementStart, sizeof(DataTypeInfo<DataType::COLOR_ARRAY>::BaseType) * finalElementCount);
		}
		case DataType::VECTOR:
		{
			auto value = getValue(name, mapVectors);
			std::memcpy(ptr, &value, sizeof(DataTypeInfo<DataType::VECTOR>::BaseType));
		}
		case DataType::VECTOR_ARRAY:
		{
			auto arr = getValue(name, mapVectorArrays);
			uint32_t finalElementCount = std::max(0u, std::min(static_cast<uint32_t>(arr.size()) - elementStart, maxElementCount));
			if (finalElementCount == 0) break;
			std::memcpy(ptr, arr.data() + elementStart, sizeof(DataTypeInfo<DataType::VECTOR_ARRAY>::BaseType) * finalElementCount);
		}
		case DataType::MATRIX:
		{
			auto value = getValue(name, mapMatrixs);
			std::memcpy(ptr, &value, sizeof(DataTypeInfo<DataType::MATRIX>::BaseType));
		}
		case DataType::MATRIX_ARRAY:
		{
			auto arr = getValue(name, mapMatrixArrays);
			uint32_t finalElementCount = std::max(0u, std::min(static_cast<uint32_t>(arr.size()) - elementStart, maxElementCount));
			if (finalElementCount == 0) break;
			std::memcpy(ptr, arr.data() + elementStart, sizeof(DataTypeInfo<DataType::MATRIX_ARRAY>::BaseType) * finalElementCount);
		}
		case DataType::TEXTURE_OFFSET:
		{
			auto value = getValue(name, mapTextureOffsets);
			std::memcpy(ptr, &value, sizeof(DataTypeInfo<DataType::TEXTURE_OFFSET>::BaseType));
		}
		case DataType::TEXTURE_SCALE:
		{
			auto value = getValue(name, mapTextureScales);
			std::memcpy(ptr, &value, sizeof(DataTypeInfo<DataType::TEXTURE_SCALE>::BaseType));
		}
		default:
			throw std::runtime_error("Invalid data type for copying its data to memory.");
		}
	}

	//-----------------this used to eliminate ide warning----------------
	template <MaterialData::DataType type>
	const typename MaterialData::DataTypeInfo<type>::ValueType& MaterialData::getDataValue(std::string name) const { return {}; }

	template<MaterialData::DataType type>
	void MaterialData::setDataValue(std::string name, const typename MaterialData::DataTypeInfo<type>::ValueType& value) {}
	//-----------------this used to eliminate ide warning----------------

	template<>
	const float &MaterialData::getDataValue<MaterialData::DataType::FLOAT>(std::string name) const
	{
		return getValue(name, mapFloats);
	}

	template<>
	void MaterialData::setDataValue<MaterialData::DataType::FLOAT>(std::string name, const float& value)
	{
		setValue(name, value, mapFloats, arrFloats);
	}

	template<>
	const std::vector<float>& MaterialData::getDataValue<MaterialData::DataType::FLOAT_ARRAY>(std::string name) const
	{
		return getValue(name, mapFloatArrays);
	}

	template<>
	void MaterialData::setDataValue<MaterialData::DataType::FLOAT_ARRAY>(std::string name, const std::vector<float>& value)
	{
		setValue(name, value, mapFloatArrays, arrFloatArrays);
	}

	template<>
	const int32_t& MaterialData::getDataValue<MaterialData::DataType::INT>(std::string name) const
	{
		return getValue(name, mapInts);
	}

	template<>
	void MaterialData::setDataValue<MaterialData::DataType::INT>(std::string name, const int32_t& value)
	{
		setValue(name, value, mapInts, arrInts);
	}

	template<>
	const std::vector<int32_t>& MaterialData::getDataValue<MaterialData::DataType::INT_ARRAY>(std::string name) const
	{
		return getValue(name, mapIntArrays);
	}

	template<>
	void MaterialData::setDataValue<MaterialData::DataType::INT_ARRAY>(std::string name, const std::vector<int32_t>& value)
	{
		setValue(name, value, mapIntArrays, arrIntArrays);
	}

	template<>
	const Color& MaterialData::getDataValue<MaterialData::DataType::COLOR>(std::string name) const
	{
		return getValue(name, mapColors);
	}

	template<>
	void MaterialData::setDataValue<MaterialData::DataType::COLOR>(std::string name, const Color& value)
	{
		setValue(name, value, mapColors, arrColors);
	}

	template<>
	const std::vector<Color>& MaterialData::getDataValue<MaterialData::DataType::COLOR_ARRAY>(std::string name) const
	{
		return getValue(name, mapColorArrays);
	}

	template<>
	void MaterialData::setDataValue<MaterialData::DataType::COLOR_ARRAY>(std::string name, const std::vector<Color>& value)
	{
		setValue(name, value, mapColorArrays, arrColorArrays);
	}

	template<>
	const Vector4& MaterialData::getDataValue<MaterialData::DataType::VECTOR>(std::string name) const
	{
		return getValue(name, mapVectors);
	}

	template<>
	void MaterialData::setDataValue<MaterialData::DataType::VECTOR>(std::string name, const Vector4& value)
	{
		setValue(name, value, mapVectors, arrVectors);
	}

	template<>
	const std::vector<Vector4>& MaterialData::getDataValue<MaterialData::DataType::VECTOR_ARRAY>(std::string name) const
	{
		return getValue(name, mapVectorArrays);
	}

	template<>
	void MaterialData::setDataValue<MaterialData::DataType::VECTOR_ARRAY>(std::string name, const std::vector<Vector4>& value)
	{
		setValue(name, value, mapVectorArrays, arrVectorArrays);
	}

	template<>
	const Matrix4x4& MaterialData::getDataValue<MaterialData::DataType::MATRIX>(std::string name) const
	{
		return getValue(name, mapMatrixs);
	}

	template<>
	void MaterialData::setDataValue<MaterialData::DataType::MATRIX>(std::string name, const Matrix4x4& value)
	{
		setValue(name, value, mapMatrixs, arrMatrixs);
	}

	template<>
	const std::vector<Matrix4x4>& MaterialData::getDataValue<MaterialData::DataType::MATRIX_ARRAY>(std::string name) const
	{
		return getValue(name, mapMatrixArrays);
	}

	template<>
	void MaterialData::setDataValue<MaterialData::DataType::MATRIX_ARRAY>(std::string name, const std::vector<Matrix4x4>& value)
	{
		setValue(name, value, mapMatrixArrays, arrMatrixArrays);
	}

	template<>
	const std::shared_ptr<Texture>& MaterialData::getDataValue<MaterialData::DataType::TEXTURE>(std::string name) const
	{
		return getValue(name, mapTextures);
	}

	template<>
	void MaterialData::setDataValue<MaterialData::DataType::TEXTURE>(std::string name, const std::shared_ptr<Texture>& value)
	{
		setValue(name, value, mapTextures, arrTextures);
	}

	template<>
	const Vector2& MaterialData::getDataValue<MaterialData::DataType::TEXTURE_OFFSET>(std::string name) const
	{
		return getValue(name, mapTextureOffsets);
	}

	template<>
	void MaterialData::setDataValue<MaterialData::DataType::TEXTURE_OFFSET>(std::string name, const Vector2& value)
	{
		setValue(name, value, mapTextureOffsets, arrTextureOffsets);
	}

	template<>
	const Vector2& MaterialData::getDataValue<MaterialData::DataType::TEXTURE_SCALE>(std::string name) const
	{
		return getValue(name, mapTextureScales);
	}

	template<>
	void MaterialData::setDataValue<MaterialData::DataType::TEXTURE_SCALE>(std::string name, const Vector2& value)
	{
		setValue(name, value, mapTextureScales, arrTextureScales);
	}
}

