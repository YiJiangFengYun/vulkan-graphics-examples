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
			return sizeof(float);
		}
		case MaterialData::DataType::FLOAT_ARRAY:
		{
			auto arr = getValue(name, mapFloatArrays, arrFloatArrays, KGS_TRUE);
			return sizeof(float) * arr.size();
		}
		case MaterialData::DataType::INT:
		{
			return sizeof(int32_t);
		}
		case MaterialData::DataType::INT_ARRAY:
		{
			auto arr = getValue(name, mapIntArrays, arrIntArrays, KGS_TRUE);
			return sizeof(int32_t) * arr.size();
		}
		case MaterialData::DataType::COLOR:
		{
			return sizeof(Color);
		}
		case MaterialData::DataType::COLOR_ARRAY:
		{
			auto arr = getValue(name, mapColorArrays, arrColorArrays, KGS_TRUE);
			return sizeof(Color) * arr.size();
		}
		case MaterialData::DataType::VECTOR:
		{
			return sizeof(Vector4);
		}
		case MaterialData::DataType::VECTOR_ARRAY:
		{
			auto arr = getValue(name, mapVectorArrays, arrVectorArrays, KGS_TRUE);
			return sizeof(Vector4) * arr.size();
		}
		case MaterialData::DataType::MATRIX:
		{
			return sizeof(Matrix4x4);
		}
		case MaterialData::DataType::MATRIX_ARRAY:
		{
			auto arr = getValue(name, mapMatrixArrays, arrMatrixArrays, KGS_TRUE);
			return sizeof(Matrix4x4) * arr.size();
		}
		case MaterialData::DataType::TEXTURE_OFFSET:
		{
			return sizeof(Vector2);
		}
		case MaterialData::DataType::TEXTURE_SCALE:
		{
			return sizeof(Vector2);
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
}
