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
			INT,
			VECTOR_2,
			VECTOR_3,
			VECTOR_4,
			COLOR_32,
			BEGIN_RANGE = FLOAT,
			END_RANGE = COLOR_32,
			RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
		};

		template <DataType type>
		struct DataTypeInfo
		{
			typedef void value_t;
			typedef void base_t;
		};

		template<>
		struct DataTypeInfo<DataType::FLOAT>
		{
			typedef float value_t;
			typedef float base_t;
		};

		template<>
		struct DataTypeInfo<DataType::INT>
		{
			typedef int32_t value_t;
			typedef int32_t  base_t;
		};

		template<>
		struct DataTypeInfo<DataType::VECTOR_2>
		{
			typedef Vector2 value_t;
			typedef Vector2 base_t;
		};

		template<>
		struct DataTypeInfo<DataType::VECTOR_3>
		{
			typedef Vector3 value_t;
			typedef Vector3 base_t;
		};

		template<>
		struct DataTypeInfo<DataType::VECTOR_4>
		{
			typedef Vector4 value_t;
			typedef Vector4 base_t;
		};

		template<>
		struct DataTypeInfo<DataType::COLOR_32>
		{
			typedef Color32 value_t;
			typedef Color32 base_t;
		};

		enum class UVType
		{
			FLOAT,
			VECTOR_2,
			VECTOR_3,
			BEGIN_RANGE = FLOAT,
			END_RANGE = VECTOR_3,
			RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
		};

		template<UVType type>
		struct UVTypeInfo
		{
			typedef void value_t;
			DataType dataType;
		};

		template<>
		struct UVTypeInfo<UVType::FLOAT>
		{
			typedef float value_t;
			DataType dataType = DataType::FLOAT;
		};

		template<>
		struct UVTypeInfo<UVType::VECTOR_2>
		{
			typedef Vector2 value_t;
			DataType dataType = DataType::VECTOR_2;
		};

		template<>
		struct UVTypeInfo<UVType::VECTOR_3>
		{
			typedef Vector3 value_t;
			DataType dataType = DataType::VECTOR_3;
		};

		std::vector<float> arrFloats;
		std::unordered_map<std::string, float> mapFloats;

		std::vector<int32_t> arrInts;
		std::unordered_map<std::string, int32_t> mapInts;

		std::vector<Vector2> arrVector2s;
		std::unordered_map <std::string, Vector2> mapVector2s;

		std::vector<Vector3> arrVector3s;
		std::unordered_map<std::string, Vector3> mapVector3s;

		std::vector<Vector4> arrVector4s;
		std::unordered_map<std::string, Vector4> mapVector4s;

		std::vector<Color32> arrColor32s;
		std::unordered_map<std::string, Color32> mapColor32s;

		template <DataType type>
		typename DataTypeInfo<type>::value_t getDataValue(std::string name);

		template <DataType type>
		void setDataValue(std::string name, typename DataTypeInfo<type>::value_t value);

		uint32_t static getDataBaseTypeSize(DataType dataType);

		uint32_t static getDataValueSize(std::string name, DataType dataType);

		void memCopyDataValue(std::string name, DataType dataType, void* dst, uint32_t offset, uint32_t maxElementCount);

	};
}

#endif // !KGS_MESH_DATA_H
