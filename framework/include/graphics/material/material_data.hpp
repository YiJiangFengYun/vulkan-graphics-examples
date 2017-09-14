#ifndef KGS_MATERIAL_DATA_H
#define KGS_MATERIAL_DATA_H

#include <algorithm>
#include <vector>
#include <unordered_map>
#include "graphics/global.hpp"
#include "graphics/util/util.hpp"
#include "graphics/texture/texture.hpp"

namespace kgs
{
	struct MaterialData
	{
		enum class DataType
		{
			FLOAT,
			FLOAT_ARRAY,
			INT,
			INT_ARRAY,
			COLOR,
			COLOR_ARRAY,
			VECTOR,
			VECTOR_ARRAY,
			MATRIX,
			MATRIX_ARRAY,
			TEXTURE,
			TEXTURE_OFFSET,
			TEXTURE_SCALE,
			BEGIN_RANGE = FLOAT,
			END_RANGE = TEXTURE_SCALE,
			RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
		};

		//--aggregations

		std::vector<float> arrFloats;
		std::unordered_map<std::string, float> mapFloats;
		std::vector<std::vector<float>> arrFloatArrays;
		std::unordered_map<std::string, std::vector<float>> mapFloatArrays;

		std::vector<int32_t> arrInts;
		std::unordered_map<std::string, int32_t> mapInts;
		std::vector<std::vector<int32_t>> arrIntArrays;
		std::unordered_map<std::string, std::vector<int32_t>> mapIntArrays;

		std::vector<Color> arrColors;
		std::unordered_map<std::string, Color> mapColors;
		std::vector<std::vector<Color>> arrColorArrays;
		std::unordered_map<std::string, std::vector<Color>> mapColorArrays;

		std::vector<Vector4> arrVectors;
		std::unordered_map<std::string, Vector4> mapVectors;
		std::vector<std::vector<Vector4>> arrVectorArrays;
		std::unordered_map<std::string, std::vector<Vector4>> mapVectorArrays;

		std::vector<Matrix4x4> arrMatrixs;
		std::unordered_map<std::string, Matrix4x4> mapMatrixs;
		std::vector<std::vector<Matrix4x4>> arrMatrixArrays;
		std::unordered_map<std::string, std::vector<Matrix4x4>> mapMatrixArrays;

		std::vector<std::shared_ptr<Texture>> arrTextures;
		std::unordered_map<std::string, std::shared_ptr<Texture>> mapTextures;
		std::vector<Vector2> arrTextureOffsets;
		std::unordered_map<std::string, Vector2> mapTextureOffsets;
		std::vector<Vector2> arrTextureScales;
		std::unordered_map<std::string, Vector2> mapTextureScales;
		//--aggregations


		//type enum to value type
		template <DataType type>
		struct ValueTypeInfo
		{
			typedef void value_t;
			typedef void base_t;
			Bool32 isArray = KGS_FALSE;
		};


		template<>
		struct ValueTypeInfo<DataType::FLOAT>
		{
			typedef float value_t;
			typedef float base_t;
			Bool32 isArray = KGS_FALSE;
		};

		template<>
		struct ValueTypeInfo<DataType::FLOAT_ARRAY>
		{
			typedef std::vector<float> value_t;
			typedef float base_t;
			Bool32 isArray = KGS_TRUE;
		};

		template<>
		struct ValueTypeInfo<DataType::INT>
		{
			typedef int32_t value_t;
			typedef int32_t base_t;
			Bool32 isArray = KGS_FALSE;
		};

		template<>
		struct ValueTypeInfo<DataType::INT_ARRAY>
		{
			typedef std::vector<int32_t> value_t;
			typedef int32_t base_t;
			Bool32 isArray = KGS_TRUE;
		};

		template<>
		struct ValueTypeInfo<DataType::COLOR>
		{
			typedef Color value_t;
			typedef Color base_t;
			Bool32 isArray = KGS_FALSE;
		};

		template<>
		struct ValueTypeInfo<DataType::COLOR_ARRAY>
		{
			typedef std::vector<Color> value_t;
			typedef Color base_t;
			Bool32 isArray = KGS_TRUE;
		};

		template<>
		struct ValueTypeInfo<DataType::VECTOR>
		{
			typedef Vector4 value_t;
			typedef Vector4 base_t;
			Bool32 isArray = KGS_FALSE;
		};

		template<>
		struct ValueTypeInfo<DataType::VECTOR_ARRAY>
		{
			typedef std::vector<Vector4> value_t;
			typedef Vector4 base_t;
			Bool32 isArray = KGS_TRUE;
		};

		template<>
		struct ValueTypeInfo<DataType::MATRIX>
		{
			typedef Matrix4x4 value_t;
			typedef Matrix4x4 base_t;
			Bool32 isArray = KGS_FALSE;
		};

		template<>
		struct ValueTypeInfo<DataType::MATRIX_ARRAY>
		{
			typedef std::vector<Matrix4x4> value_t;
			typedef Matrix4x4 base_t;
			Bool32 isArray = KGS_TRUE;
		};

		template<>
		struct ValueTypeInfo<DataType::TEXTURE>
		{
			typedef std::shared_ptr<Texture> value_t;
			typedef std::shared_ptr<Texture> base_t;
			Bool32 isArray = KGS_FALSE;
		};

		template<>
		struct ValueTypeInfo<DataType::TEXTURE_OFFSET>
		{
			typedef Vector2 value_t;
			typedef Vector2 base_t;
			Bool32 isArray = KGS_FALSE;
		};

		template<>
		struct ValueTypeInfo<DataType::TEXTURE_SCALE>
		{
			typedef Vector2 value_t;
			typedef Vector2 base_t;
			Bool32 isArray = KGS_FALSE;
		};

		template <DataType type>
		typename ValueTypeInfo<type>::value_t getDataValue(std::string name) 
		{
			return {};
		}

		uint32_t static getDataBaseType(DataType dataType);

		uint32_t getDataValueSize(std::string name, DataType dataType);

		void memCopyDataValue(std::string name, DataType dataType, void* dst, uint32_t offset, uint32_t maxElementCount);
	};

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
	int32_t MaterialData::getDataValue<MaterialData::DataType::INT>(std::string name)
	{
		return getValue(name, mapInts, arrInts);
	}

	template<>
	std::vector<int32_t> MaterialData::getDataValue<MaterialData::DataType::INT_ARRAY>(std::string name)
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
} //namespace kgs

#endif // !KGS_MATERIAL_DATA_H
