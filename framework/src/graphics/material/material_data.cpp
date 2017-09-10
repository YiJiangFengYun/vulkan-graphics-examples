#include "graphics/material/material_data.hpp"

namespace kgs
{
	uint32_t MaterialData::getDataValueSize(std::string name, DataType dataType)
	{
		switch (dataType)
		{
		case MaterialData::DataType::FLOAT:
		{
			return sizeof(getValue(name, mapFloats, arrFloats, KGS_TRUE));
			break;
		}
		case MaterialData::DataType::FLOAT_ARRAY:
		{
			return sizeof(getValue(name, mapFloatArrays, arrFloatArrays, KGS_TRUE));
			break;
		}
		case MaterialData::DataType::INT:
		{
			return sizeof(getValue(name, mapInts, arrInts, KGS_TRUE));
			break;
		}
		case MaterialData::DataType::INT_ARRAY:
		{
			return sizeof(getValue(name, mapIntArrays, arrIntArrays, KGS_TRUE));
			break;
		}
		case MaterialData::DataType::COLOR:
		{
			return sizeof(getValue(name, mapColors, arrColors, KGS_TRUE));
			break;
		}
		case MaterialData::DataType::COLOR_ARRAY:
		{
			return sizeof(getValue(name, mapColorArrays, arrColorArrays, KGS_TRUE));
			break;
		}
		case MaterialData::DataType::VECTOR:
		{
			return sizeof(getValue(name, mapVectors, arrVectors, KGS_TRUE));
			break;
		}
		case MaterialData::DataType::VECTOR_ARRAY:
		{
			return sizeof(getValue(name, mapVectorArrays, arrVectorArrays, KGS_TRUE));
			break;
		}
		case MaterialData::DataType::MATRIX:
		{
			return sizeof(getValue(name, mapMatrixs, arrMatrixs, KGS_TRUE));
			break;
		}
		case MaterialData::DataType::MATRIX_ARRAY:
		{
			return sizeof(getValue(name, mapMatrixArrays, arrMatrixArrays, KGS_TRUE));
			break;
		}
		case MaterialData::DataType::TEXTURE_OFFSET:
		{
			return sizeof(getValue(name, mapTextureOffsets, arrTextureOffsets, KGS_TRUE));
			break;
		}
		case MaterialData::DataType::TEXTURE_SCALE:
		{
			return sizeof(getValue(name, mapTextureScales, arrTextureScales, KGS_TRUE));
			break;
		}
		default:
			throw std::runtime_error("Invalid data type for getting its used memory size");
		}
	}

	template<>
	float MaterialData::getDataValue<MaterialData::DataType::FLOAT>(std::string name)
	{
		return getValue(name, mapFloats, arrFloats);
	}

	template<>
	std::vector<float> MaterialData::getDataValue<MaterialData::DataType::FLOAT_ARRAY>(std::string name)
	{
		return getValue(name, mapFloatArrays, arrFloatArrays);
	}

	template<>
	int MaterialData::getDataValue<MaterialData::DataType::INT>(std::string name)
	{
		return getValue(name, mapInts, arrInts);
	}

	template<>
	std::vector<int> MaterialData::getDataValue<MaterialData::DataType::INT_ARRAY>(std::string name)
	{
		return getValue(name, mapIntArrays, arrIntArrays);
	}

	template<>
	Color MaterialData::getDataValue<MaterialData::DataType::COLOR>(std::string name)
	{
		return getValue(name, mapColors, arrColors);
	}

	template<>
	std::vector<Color> MaterialData::getDataValue<MaterialData::DataType::COLOR_ARRAY>(std::string name)
	{
		return getValue(name, mapColorArrays, arrColorArrays);
	}

	template<>
	Vector4 MaterialData::getDataValue<MaterialData::DataType::VECTOR>(std::string name)
	{
		return getValue(name, mapVectors, arrVectors);
	}

	template<>
	std::vector<Vector4> MaterialData::getDataValue<MaterialData::DataType::VECTOR_ARRAY>(std::string name)
	{
		return getValue(name, mapVectorArrays, arrVectorArrays);
	}

	template<>
	Matrix4x4 MaterialData::getDataValue<MaterialData::DataType::MATRIX>(std::string name)
	{
		return getValue(name, mapMatrixs, arrMatrixs);
	}

	template<>
	std::vector<Matrix4x4> MaterialData::getDataValue<MaterialData::DataType::MATRIX_ARRAY>(std::string name)
	{
		return getValue(name, mapMatrixArrays, arrMatrixArrays);
	}

	template<>
	std::shared_ptr<Texture> MaterialData::getDataValue<MaterialData::DataType::TEXTURE>(std::string name)
	{
		return getValue(name, mapTextures, arrTextures);
	}

	template<>
	Vector2 MaterialData::getDataValue<MaterialData::DataType::TEXTURE_OFFSET>(std::string name)
	{
		return getValue(name, mapTextureOffsets, arrTextureOffsets);
	}

	template<>
	Vector2 MaterialData::getDataValue<MaterialData::DataType::TEXTURE_SCALE>(std::string name)
	{
		return getValue(name, mapTextureScales, arrTextureScales);
	}
}

