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
	enum class MaterialShowType
	{
		BACKGROUND,
		OPAQUE,
		TRANSPARENT,
		OVERLAY,
		BEGIN_RANGE = BACKGROUND,
		END_RANGE = OVERLAY,
		RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
	};

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
			typedef void ValueType;
			typedef void BaseType;
			static const Bool32 IS_ARRAY = KGS_FALSE;
		};


		template<>
		struct DataTypeInfo<DataType::FLOAT>
		{
			typedef float ValueType;
			typedef float BaseType;
			const static Bool32 IS_ARRAY = KGS_FALSE;
		};

		template<>
		struct DataTypeInfo<DataType::FLOAT_ARRAY>
		{
			typedef std::vector<float> ValueType;
			typedef float BaseType;
			const static Bool32 IS_ARRAY = KGS_TRUE;
		};

		template<>
		struct DataTypeInfo<DataType::INT>
		{
			typedef int32_t ValueType;
			typedef int32_t BaseType;
			const static Bool32 IS_ARRAY = KGS_FALSE;
		};

		template<>
		struct DataTypeInfo<DataType::INT_ARRAY>
		{
			typedef std::vector<int32_t> ValueType;
			typedef int32_t BaseType;
			const static Bool32 IS_ARRAY = KGS_TRUE;
		};

		template<>
		struct DataTypeInfo<DataType::COLOR>
		{
			typedef Color ValueType;
			typedef Color BaseType;
			const static Bool32 IS_ARRAY = KGS_FALSE;
		};

		template<>
		struct DataTypeInfo<DataType::COLOR_ARRAY>
		{
			typedef std::vector<Color> ValueType;
			typedef Color BaseType;
			const static Bool32 IS_ARRAY = KGS_TRUE;
		};

		template<>
		struct DataTypeInfo<DataType::VECTOR>
		{
			typedef Vector4 ValueType;
			typedef Vector4 BaseType;
			const static Bool32 IS_ARRAY = KGS_FALSE;
		};

		template<>
		struct DataTypeInfo<DataType::VECTOR_ARRAY>
		{
			typedef std::vector<Vector4> ValueType;
			typedef Vector4 BaseType;
			const static Bool32 IS_ARRAY = KGS_TRUE;
		};

		template<>
		struct DataTypeInfo<DataType::MATRIX>
		{
			typedef Matrix4x4 ValueType;
			typedef Matrix4x4 BaseType;
			const static Bool32 IS_ARRAY = KGS_FALSE;
		};

		template<>
		struct DataTypeInfo<DataType::MATRIX_ARRAY>
		{
			typedef std::vector<Matrix4x4> ValueType;
			typedef Matrix4x4 BaseType;
			const static Bool32 IS_ARRAY = KGS_TRUE;
		};

		template<>
		struct DataTypeInfo<DataType::TEXTURE>
		{
			typedef std::shared_ptr<Texture> ValueType;
			typedef std::shared_ptr<Texture> BaseType;
			const static Bool32 IS_ARRAY = KGS_FALSE;
		};

		template<>
		struct DataTypeInfo<DataType::TEXTURE_OFFSET>
		{
			typedef Vector2 ValueType;
			typedef Vector2 BaseType;
			const static Bool32 IS_ARRAY = KGS_FALSE;
		};

		template<>
		struct DataTypeInfo<DataType::TEXTURE_SCALE>
		{
			typedef Vector2 ValueType;
			typedef Vector2 BaseType;
			const static Bool32 IS_ARRAY = KGS_FALSE;
		};

		std::vector<std::string> arrFloats;
		std::unordered_map<std::string, float> mapFloats;
		std::vector<std::string> arrFloatArrays;
		std::unordered_map<std::string, std::vector<float>> mapFloatArrays;

		std::vector<std::string> arrInts;
		std::unordered_map<std::string, int32_t> mapInts;
		std::vector<std::string> arrIntArrays;
		std::unordered_map<std::string, std::vector<int32_t>> mapIntArrays;

		std::vector<std::string> arrColors;
		std::unordered_map<std::string, Color> mapColors;
		std::vector<std::string> arrColorArrays;
		std::unordered_map<std::string, std::vector<Color>> mapColorArrays;

		std::vector<std::string> arrVectors;
		std::unordered_map<std::string, Vector4> mapVectors;
		std::vector<std::string> arrVectorArrays;
		std::unordered_map<std::string, std::vector<Vector4>> mapVectorArrays;

		std::vector<std::string> arrMatrixs;
		std::unordered_map<std::string, Matrix4x4> mapMatrixs;
		std::vector<std::string> arrMatrixArrays;
		std::unordered_map<std::string, std::vector<Matrix4x4>> mapMatrixArrays;

		std::vector<std::string> arrTextures;
		std::unordered_map<std::string, std::shared_ptr<Texture>> mapTextures;
		std::vector<std::string> arrTextureOffsets;
		std::unordered_map<std::string, Vector2> mapTextureOffsets;
		std::vector<std::string> arrTextureScales;
		std::unordered_map<std::string, Vector2> mapTextureScales;

		template <DataType type>
		const typename DataTypeInfo<type>::ValueType &getDataValue(std::string name) const;

		template<DataType type>
		void setDataValue(std::string name, const typename DataTypeInfo<type>::ValueType& value);

		uint32_t static getDataBaseTypeSize(DataType dataType);

		uint32_t getDataValueSize(std::string name, DataType dataType);

		void memCopyDataValue(std::string name, DataType dataType, void* dst, uint32_t offset, uint32_t elementStart, uint32_t maxElementCount);
	};
} //namespace kgs

#endif // !KGS_MATERIAL_DATA_H
