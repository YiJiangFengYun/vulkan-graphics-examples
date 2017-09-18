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

		//type enum to value type
		template <DataType type>
		struct DataTypeInfo
		{
			typedef void value_t;
			typedef void base_t;
			Bool32 isArray = KGS_FALSE;
		};


		template<>
		struct DataTypeInfo<DataType::FLOAT>
		{
			typedef float value_t;
			typedef float base_t;
			Bool32 isArray = KGS_FALSE;
		};

		template<>
		struct DataTypeInfo<DataType::FLOAT_ARRAY>
		{
			typedef std::vector<float> value_t;
			typedef float base_t;
			Bool32 isArray = KGS_TRUE;
		};

		template<>
		struct DataTypeInfo<DataType::INT>
		{
			typedef int32_t value_t;
			typedef int32_t base_t;
			Bool32 isArray = KGS_FALSE;
		};

		template<>
		struct DataTypeInfo<DataType::INT_ARRAY>
		{
			typedef std::vector<int32_t> value_t;
			typedef int32_t base_t;
			Bool32 isArray = KGS_TRUE;
		};

		template<>
		struct DataTypeInfo<DataType::COLOR>
		{
			typedef Color value_t;
			typedef Color base_t;
			Bool32 isArray = KGS_FALSE;
		};

		template<>
		struct DataTypeInfo<DataType::COLOR_ARRAY>
		{
			typedef std::vector<Color> value_t;
			typedef Color base_t;
			Bool32 isArray = KGS_TRUE;
		};

		template<>
		struct DataTypeInfo<DataType::VECTOR>
		{
			typedef Vector4 value_t;
			typedef Vector4 base_t;
			Bool32 isArray = KGS_FALSE;
		};

		template<>
		struct DataTypeInfo<DataType::VECTOR_ARRAY>
		{
			typedef std::vector<Vector4> value_t;
			typedef Vector4 base_t;
			Bool32 isArray = KGS_TRUE;
		};

		template<>
		struct DataTypeInfo<DataType::MATRIX>
		{
			typedef Matrix4x4 value_t;
			typedef Matrix4x4 base_t;
			Bool32 isArray = KGS_FALSE;
		};

		template<>
		struct DataTypeInfo<DataType::MATRIX_ARRAY>
		{
			typedef std::vector<Matrix4x4> value_t;
			typedef Matrix4x4 base_t;
			Bool32 isArray = KGS_TRUE;
		};

		template<>
		struct DataTypeInfo<DataType::TEXTURE>
		{
			typedef std::shared_ptr<Texture> value_t;
			typedef std::shared_ptr<Texture> base_t;
			Bool32 isArray = KGS_FALSE;
		};

		template<>
		struct DataTypeInfo<DataType::TEXTURE_OFFSET>
		{
			typedef Vector2 value_t;
			typedef Vector2 base_t;
			Bool32 isArray = KGS_FALSE;
		};

		template<>
		struct DataTypeInfo<DataType::TEXTURE_SCALE>
		{
			typedef Vector2 value_t;
			typedef Vector2 base_t;
			Bool32 isArray = KGS_FALSE;
		};

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

		template <DataType type>
		typename DataTypeInfo<type>::value_t getDataValue(std::string name);

		template<DataType type>
		void setDataValue(std::string name, typename DataTypeInfo<type>::value_t value);

		uint32_t static getDataBaseType(DataType dataType);

		uint32_t getDataValueSize(std::string name, DataType dataType);

		void memCopyDataValue(std::string name, DataType dataType, void* dst, uint32_t offset, uint32_t maxElementCount);
	};
} //namespace kgs

#endif // !KGS_MATERIAL_DATA_H
