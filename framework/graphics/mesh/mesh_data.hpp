#ifndef KGS_MESH_DATA_H
#define KGS_MESH_DATA_H

#include <cstdint>
#include <vector>
#include <unordered_map>
#include "graphics/global.hpp"
#include "graphics/util/util.hpp"

#define KGS_VERTEX_POSITION_NAME "_Position"
#define KGS_VERTEX_COLOR_NAME "_Color"
#define KGS_VERTEX_NORMAL_NAME "_Normal"
#define KGS_VERTEX_TANGENT_NAME "_Tangent"
#define KGS_VERTEX_UV0_NAME "_UV_0"
#define KGS_VERTEX_UV1_NAME "_UV_1"
#define KGS_VERTEX_UV2_NAME "_UV_2"
#define KGS_VERTEX_UV3_NAME "_UV_3"

#define KGS_VERTEX_BINDING_PRIORITY_POSITION 0u
#define KGS_VERTEX_BINDING_PRIORITY_COLOR 1u
#define KGS_VERTEX_BINDING_PRIORITY_NORMAL 2u
#define KGS_VERTEX_BINDING_PRIORITY_TANGENT 3u
#define KGS_VERTEX_BINDING_PRIORITY_UV0 4u
#define KGS_VERTEX_BINDING_PRIORITY_UV1 5u
#define KGS_VERTEX_BINDING_PRIORITY_UV2 6u
#define KGS_VERTEX_BINDING_PRIORITY_UV3 7u
#define KGS_VERTEX_BINDING_PRIORITY_OTHER_MIN 8u

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
			const vk::Format static BASE_FORMAT = vk::Format::eR8G8B8A8Unorm;
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
		std::unordered_map<std::string, std::vector<Vector3>> mapVector3Arrays;

		//std::vector<Vector4> arrVector4s;
		std::unordered_map<std::string, Vector4> mapVector4s;

		//std::vector<std::vector<Vector4>> arrVector4Arrays;
		std::unordered_map<std::string, std::vector<Vector4>> mapVector4Arrays;

		//std::vector<Color32> arrColor32s;
		std::unordered_map<std::string, Color32> mapColor32s;

		//std::vector<std::vector<Color32>> arrColor32Arrays;
		std::unordered_map<std::string, std::vector<Color32>> mapColor32Arrays;

		template <DataType type>
		const typename DataTypeInfo<type>::ValueType& getDataValue(std::string name) const;

		template <DataType type>
		void setDataValue(std::string name, const typename DataTypeInfo<type>::ValueType& value);

		uint32_t static getDataBaseTypeSize(DataType dataType);

		uint32_t getDataValueSize(std::string name, DataType dataType);

		void memCopyDataValue(std::string name, DataType dataType, void* dst, uint32_t offset, uint32_t elementStart, uint32_t maxElementCount);

	};
}

#include "graphics/mesh/mesh_data.inl"

#endif // !KGS_MESH_DATA_H