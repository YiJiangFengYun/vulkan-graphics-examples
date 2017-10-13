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
			FLOAT_ARRAY,
			INT_ARRAY,
			VECTOR_2_ARRAY,
			VECTOR_3_ARRAY,
			VECTOR_4_ARRAY,
			COLOR_32_ARRAY,
			BEGIN_RANGE = FLOAT_ARRAY,
			END_RANGE = COLOR_32_ARRAY,
			RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
		};

		template <DataType type>
		struct DataTypeInfo
		{
			typedef void ValueType;
			typedef void BaseType;
			const vk::Format static BASE_FORMAT = vk::Format::eUndefined;
		};

		template<>
		struct DataTypeInfo<DataType::FLOAT_ARRAY>
		{
			typedef std::vector<float> ValueType;
			typedef float BaseType;
			const vk::Format static BASE_FORMAT = vk::Format::eR32Sfloat;
		};

		template<>
		struct DataTypeInfo<DataType::INT_ARRAY>
		{
			typedef std::vector<int32_t> ValueType;
			typedef int32_t  BaseType;
			const vk::Format static BASE_FORMAT = vk::Format::eR32Sint;
		};

		template<>
		struct DataTypeInfo<DataType::VECTOR_2_ARRAY>
		{
			typedef std::vector<Vector2> ValueType;
			typedef Vector2 BaseType;
			const vk::Format static BASE_FORMAT = vk::Format::eR32G32Sfloat;
		};

		template<>
		struct DataTypeInfo<DataType::VECTOR_3_ARRAY>
		{
			typedef std::vector<Vector3> ValueType;
			typedef Vector3 BaseType;
			const vk::Format static BASE_FORMAT = vk::Format::eR32G32B32Sfloat;
		};

		template<>
		struct DataTypeInfo<DataType::VECTOR_4_ARRAY>
		{
			typedef std::vector<Vector4> ValueType;
			typedef Vector4 BaseType;
			const vk::Format static BASE_FORMAT = vk::Format::eR32G32B32A32Sfloat;
		};

		template<>
		struct DataTypeInfo<DataType::COLOR_32_ARRAY>
		{
			typedef std::vector<Color32> ValueType;
			typedef Color32 BaseType;
			const vk::Format static BASE_FORMAT = vk::Format::eR8G8B8A8Snorm;
		};

		inline static vk::Format getBaseFormatWithDataType(DataType dataType)
		{
			switch (dataType)
			{
			case DataType::FLOAT_ARRAY:
			{
				return DataTypeInfo<DataType::FLOAT_ARRAY>::BASE_FORMAT;
				break;
			}
			case DataType::INT_ARRAY:
			{
				return DataTypeInfo<DataType::INT_ARRAY>::BASE_FORMAT;
				break;
			}
			case DataType::VECTOR_2_ARRAY:
			{
				return DataTypeInfo<DataType::VECTOR_2_ARRAY>::BASE_FORMAT;
				break;
			}
			case DataType::VECTOR_3_ARRAY:
			{
				return DataTypeInfo<DataType::VECTOR_3_ARRAY>::BASE_FORMAT;
				break;
			}
			case DataType::VECTOR_4_ARRAY:
			{
				return DataTypeInfo<DataType::VECTOR_4_ARRAY>::BASE_FORMAT;
				break;
			}
			case DataType::COLOR_32_ARRAY:
			{
				return DataTypeInfo<DataType::COLOR_32_ARRAY>::BASE_FORMAT;
				break;
			}
			default:
				throw std::runtime_error("Can't get base format with data type.");
				break;
			}
		}

		//std::vector<float> arrFloats;
		std::unordered_map<std::string, float> mapFloats;

		//std::vector<std::vector<float>> arrFloatArrays;
		std::unordered_map<std::string, std::vector<float>> mapFloatArrays;

		//std::vector<int32_t> arrInts;
		std::unordered_map<std::string, int32_t> mapInts;

		//std::vector<std::vector<int32_t>> arrIntArrays;
		std::unordered_map<std::string, std::vector<int32_t>> mapIntArrays;

		//std::vector<Vector2> arrVector2s;
		std::unordered_map <std::string, Vector2> mapVector2s;

		//std::vector<std::vector<Vector2>> arrVector2Arrays;
		std::unordered_map <std::string, std::vector<Vector2>> mapVector2Arrays;

		//std::vector<Vector3> arrVector3s;
		std::unordered_map<std::string, Vector3> mapVector3s;

		//std::vector<std::vector<Vector3>> arrVector3Arrays;
		std::unordered_map <std::string, std::vector<Vector3>> mapVector3Arrays;

		//std::vector<Vector4> arrVector4s;
		std::unordered_map<std::string, Vector4> mapVector4s;

		//std::vector<std::vector<Vector4>> arrVector4Arrays;
		std::unordered_map<std::string, std::vector<Vector4>> mapVector4Arrays;

		//std::vector<Color32> arrColor32s;
		std::unordered_map<std::string, Color32> mapColor32s;

		//std::vector<std::vector<Color32>> arrColor32Arrays;
		std::unordered_map<std::string, std::vector<Color32>> mapColor32Arrays;

		template <DataType type>
		typename DataTypeInfo<type>::ValueType& getDataValue(std::string name);

		template <DataType type>
		void setDataValue(std::string name, typename DataTypeInfo<type>::ValueType& value);

		uint32_t static getDataBaseTypeSize(DataType dataType);

		uint32_t getDataValueSize(std::string name, DataType dataType);

		void memCopyDataValue(std::string name, DataType dataType, void* dst, uint32_t offset, uint32_t elementStart, uint32_t maxElementCount);

	};
}

#endif // !KGS_MESH_DATA_H
