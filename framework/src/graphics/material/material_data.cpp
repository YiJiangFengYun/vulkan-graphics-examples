#include "graphics/material/material_data.hpp"

namespace kgs
{
	uint32_t MaterialData::getDataBaseType(DataType dataType)
	{
		switch (dataType)
		{
		case MaterialData::DataType::FLOAT:
		case MaterialData::DataType::FLOAT_ARRAY:
		{
			return sizeof(float);
		}
		case MaterialData::DataType::INT:
		case MaterialData::DataType::INT_ARRAY:
		{
			return sizeof(int32_t);
		}
		case MaterialData::DataType::COLOR:
		case MaterialData::DataType::COLOR_ARRAY:
		{
			return sizeof(Color);
		}
		case MaterialData::DataType::VECTOR:
		case MaterialData::DataType::VECTOR_ARRAY:
		{
			return sizeof(Vector4);
		}
		case MaterialData::DataType::MATRIX:
		case MaterialData::DataType::MATRIX_ARRAY:
		{
			return sizeof(Matrix4x4);
		}
		case MaterialData::DataType::TEXTURE_OFFSET:
		case MaterialData::DataType::TEXTURE_SCALE:
		{
			return sizeof(Vector2);
		}
		default:
			throw std::runtime_error("Invalid data type for getting its used memory size");
		}
	}

	uint32_t MaterialData::getDataValueSize(std::string name, DataType dataType)
	{
		switch (dataType)
		{
		case MaterialData::DataType::FLOAT:
		{
			return static_cast<uint32_t>(sizeof(float));
		}
		case MaterialData::DataType::FLOAT_ARRAY:
		{
			auto arr = getValue(name, mapFloatArrays, arrFloatArrays, KGS_TRUE);
			return static_cast<uint32_t>(sizeof(float) * arr.size());
		}
		case MaterialData::DataType::INT:
		{
			return static_cast<uint32_t>(sizeof(int32_t));
		}
		case MaterialData::DataType::INT_ARRAY:
		{
			auto arr = getValue(name, mapIntArrays, arrIntArrays, KGS_TRUE);
			return static_cast<uint32_t>(sizeof(int32_t) * arr.size());
		}
		case MaterialData::DataType::COLOR:
		{
			return static_cast<uint32_t>(sizeof(Color));
		}
		case MaterialData::DataType::COLOR_ARRAY:
		{
			auto arr = getValue(name, mapColorArrays, arrColorArrays, KGS_TRUE);
			return static_cast<uint32_t>(sizeof(Color) * arr.size());
		}
		case MaterialData::DataType::VECTOR:
		{
			return static_cast<uint32_t>(sizeof(Vector4));
		}
		case MaterialData::DataType::VECTOR_ARRAY:
		{
			auto arr = getValue(name, mapVectorArrays, arrVectorArrays, KGS_TRUE);
			return static_cast<uint32_t>(sizeof(Vector4) * arr.size());
		}
		case MaterialData::DataType::MATRIX:
		{
			return static_cast<uint32_t>(sizeof(Matrix4x4));
		}
		case MaterialData::DataType::MATRIX_ARRAY:
		{
			auto arr = getValue(name, mapMatrixArrays, arrMatrixArrays, KGS_TRUE);
			return static_cast<uint32_t>(sizeof(Matrix4x4) * arr.size());
		}
		case MaterialData::DataType::TEXTURE_OFFSET:
		{
			return static_cast<uint32_t>(sizeof(Vector2));
		}
		case MaterialData::DataType::TEXTURE_SCALE:
		{
			return static_cast<uint32_t>(sizeof(Vector2));
		}
		default:
			throw std::runtime_error("Invalid data type for getting its used memory size");
		}
	}

	void MaterialData::memCopyDataValue(std::string name, DataType dataType, void* dst, uint32_t offset, uint32_t maxElementCount)
	{
		char *ptr = static_cast<char *>(dst);
		ptr += offset;
		switch (dataType)
		{
		case MaterialData::DataType::FLOAT:
		{
			auto value = getValue(name, mapFloats, arrFloats, KGS_TRUE);
			std::memcpy(ptr, &value, sizeof(float));
		}
		case MaterialData::DataType::FLOAT_ARRAY:
		{
			auto arr = getValue(name, mapFloatArrays, arrFloatArrays, KGS_TRUE);
			std::memcpy(ptr, arr.data(), sizeof(float) * std::min(static_cast<uint32_t>(arr.size()), maxElementCount));
		}
		case MaterialData::DataType::INT:
		{
			auto value = getValue(name, mapInts, arrInts, KGS_TRUE);
			std::memcpy(ptr, &value, sizeof(int32_t));
		}
		case MaterialData::DataType::INT_ARRAY:
		{
			auto arr = getValue(name, mapIntArrays, arrIntArrays, KGS_TRUE);
			std::memcpy(ptr, arr.data(), sizeof(int32_t) * std::min(static_cast<uint32_t>(arr.size()), maxElementCount));
		}
		case MaterialData::DataType::COLOR:
		{
			auto value = getValue(name, mapColors, arrColors, KGS_TRUE);
			std::memcpy(ptr, &value, sizeof(Color));
		}
		case MaterialData::DataType::COLOR_ARRAY:
		{
			auto arr = getValue(name, mapColorArrays, arrColorArrays, KGS_TRUE);
			std::memcpy(ptr, arr.data(), sizeof(Color) * std::min(static_cast<uint32_t>(arr.size()), maxElementCount));
		}
		case MaterialData::DataType::VECTOR:
		{
			auto value = getValue(name, mapVectors, arrVectors, KGS_TRUE);
			std::memcpy(ptr, &value, sizeof(Vector4));
		}
		case MaterialData::DataType::VECTOR_ARRAY:
		{
			auto arr = getValue(name, mapVectorArrays, arrVectorArrays, KGS_TRUE);
			std::memcpy(ptr, arr.data(), sizeof(Vector4) * std::min(static_cast<uint32_t>(arr.size()), maxElementCount));
		}
		case MaterialData::DataType::MATRIX:
		{
			auto value = getValue(name, mapMatrixs, arrMatrixs, KGS_TRUE);
			std::memcpy(ptr, &value, sizeof(Matrix4x4));
		}
		case MaterialData::DataType::MATRIX_ARRAY:
		{
			auto arr = getValue(name, mapMatrixArrays, arrMatrixArrays, KGS_TRUE);
			std::memcpy(ptr, arr.data(), sizeof(Matrix4x4) * std::min(static_cast<uint32_t>(arr.size()), maxElementCount));
		}
		case MaterialData::DataType::TEXTURE_OFFSET:
		{
			auto value = getValue(name, mapTextureOffsets, arrTextureOffsets, KGS_TRUE);
			std::memcpy(ptr, &value, sizeof(Vector2));
		}
		case MaterialData::DataType::TEXTURE_SCALE:
		{
			auto value = getValue(name, mapTextureScales, arrTextureScales, KGS_TRUE);
			std::memcpy(ptr, &value, sizeof(Vector2));
		}
		default:
			throw std::runtime_error("Invalid data type for copying its data to memory.");
		}
	}

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

