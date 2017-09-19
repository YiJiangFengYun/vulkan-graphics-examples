#ifndef KGS_MESH_DATA_H
#define KGS_MESH_DATA_H

#include <cstdint>
#include <vector>
#include <unordered_map>
#include "graphics/global.hpp"
#include "graphics/util/util.hpp"

namespace kgs
{
	struct MeshData
	{
		enum class DataType
		{
			FLOAT,
			FLOAT_ARRAY,
			INT,
			INT_ARRAY,
			VECTOR_2,
			VECTOR_2_ARRAY,
			VECTOR_3,
			VECTOR_3_ARRAY,
			VECTOR_4,
			VECTOR_4_ARRAY,
			COLOR_32,
			COLOR_32_ARRAY,
			BEGIN_RANGE = FLOAT,
			END_RANGE = COLOR_32_ARRAY,
			RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
		};

		template <DataType type>
		struct DataTypeInfo
		{
			typedef void ValueType;
			typedef void BaseType;
		};

		template<>
		struct DataTypeInfo<DataType::FLOAT>
		{
			typedef float ValueType;
			typedef float BaseType;
		};

		template<>
		struct DataTypeInfo<DataType::FLOAT_ARRAY>
		{
			typedef std::vector<float> ValueType;
			typedef float BaseType;
		};

		template<>
		struct DataTypeInfo<DataType::INT>
		{
			typedef int32_t ValueType;
			typedef int32_t  BaseType;
		};

		template<>
		struct DataTypeInfo<DataType::INT_ARRAY>
		{
			typedef std::vector<int32_t> ValueType;
			typedef int32_t  BaseType;
		};

		template<>
		struct DataTypeInfo<DataType::VECTOR_2>
		{
			typedef Vector2 ValueType;
			typedef Vector2 BaseType;
		};

		template<>
		struct DataTypeInfo<DataType::VECTOR_2_ARRAY>
		{
			typedef std::vector<Vector2> ValueType;
			typedef Vector2 BaseType;
		};

		template<>
		struct DataTypeInfo<DataType::VECTOR_3>
		{
			typedef Vector3 ValueType;
			typedef Vector3 BaseType;
		};

		template<>
		struct DataTypeInfo<DataType::VECTOR_3_ARRAY>
		{
			typedef std::vector<Vector3> ValueType;
			typedef Vector3 BaseType;
		};

		template<>
		struct DataTypeInfo<DataType::VECTOR_4>
		{
			typedef Vector4 ValueType;
			typedef Vector4 BaseType;
		};

		template<>
		struct DataTypeInfo<DataType::VECTOR_4_ARRAY>
		{
			typedef std::vector<Vector4> ValueType;
			typedef Vector4 BaseType;
		};

		template<>
		struct DataTypeInfo<DataType::COLOR_32>
		{
			typedef Color32 ValueType;
			typedef Color32 BaseType;
		};

		template<>
		struct DataTypeInfo<DataType::COLOR_32_ARRAY>
		{
			typedef std::vector<Color32> ValueType;
			typedef Color32 BaseType;
		};

		std::vector<float> arrFloats;
		std::unordered_map<std::string, float> mapFloats;

		std::vector<std::vector<float>> arrFloatArrays;
		std::unordered_map<std::string, std::vector<float>> mapFloatArrays;

		std::vector<int32_t> arrInts;
		std::unordered_map<std::string, int32_t> mapInts;

		std::vector<std::vector<int32_t>> arrIntArrays;
		std::unordered_map<std::string, std::vector<int32_t>> mapIntArrays;

		std::vector<Vector2> arrVector2s;
		std::unordered_map <std::string, Vector2> mapVector2s;

		std::vector<std::vector<Vector2>> arrVector2Arrays;
		std::unordered_map <std::string, std::vector<Vector2>> mapVector2Arrays;

		std::vector<Vector3> arrVector3s;
		std::unordered_map<std::string, Vector3> mapVector3s;

		std::vector<std::vector<Vector3>> arrVector3Arrays;
		std::unordered_map <std::string, std::vector<Vector3>> mapVector3Arrays;

		std::vector<Vector4> arrVector4s;
		std::unordered_map<std::string, Vector4> mapVector4s;

		std::vector<std::vector<Vector4>> arrVector4Arrays;
		std::unordered_map<std::string, std::vector<Vector4>> mapVector4Arrays;

		std::vector<Color32> arrColor32s;
		std::unordered_map<std::string, Color32> mapColor32s;

		std::vector<std::vector<Color32>> arrColor32Arrays;
		std::unordered_map<std::string, std::vector<Color32>> mapColor32Arrays;

		template <DataType type>
		typename DataTypeInfo<type>::ValueType getDataValue(std::string name);

		template <DataType type>
		void setDataValue(std::string name, typename DataTypeInfo<type>::ValueType value);

		uint32_t static getDataBaseTypeSize(DataType dataType);

		uint32_t getDataValueSize(std::string name, DataType dataType);

		void memCopyDataValue(std::string name, DataType dataType, void* dst, uint32_t offset, uint32_t maxElementCount);

	};
}

#endif // !KGS_MESH_DATA_H
