#ifndef VG_MESH_DATA_H
#define VG_MESH_DATA_H

#include <cstdint>
#include <vector>
#include <unordered_map>
#include "graphics/global.hpp"
#include "graphics/util/util.hpp"

#define VG_VERTEX_POSITION_NAME "_Position"
#define VG_VERTEX_COLOR_NAME "_Color"
#define VG_VERTEX_NORMAL_NAME "_Normal"
#define VG_VERTEX_TANGENT_NAME "_Tangent"
#define VG_VERTEX_UV0_NAME "_UV_0"
#define VG_VERTEX_UV1_NAME "_UV_1"
#define VG_VERTEX_UV2_NAME "_UV_2"
#define VG_VERTEX_UV3_NAME "_UV_3"

#define VG_VERTEX_BINDING_PRIORITY_POSITION 0u
#define VG_VERTEX_BINDING_PRIORITY_COLOR 1u
#define VG_VERTEX_BINDING_PRIORITY_NORMAL 2u
#define VG_VERTEX_BINDING_PRIORITY_TANGENT 3u
#define VG_VERTEX_BINDING_PRIORITY_UV0 4u
#define VG_VERTEX_BINDING_PRIORITY_UV1 5u
#define VG_VERTEX_BINDING_PRIORITY_UV2 6u
#define VG_VERTEX_BINDING_PRIORITY_UV3 7u
#define VG_VERTEX_BINDING_PRIORITY_OTHER_MIN 8u

namespace vg
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
			using ValueType = void;
			using BaseType = void ;
			const vk::Format static BASE_FORMAT = vk::Format::eUndefined;
		};

		template<>
		struct DataTypeInfo<DataType::FLOAT_ARRAY>
		{
			using ValueType = std::vector<float>;
			using BaseType = float;
			const vk::Format static BASE_FORMAT = vk::Format::eR32Sfloat;
		};

		template<>
		struct DataTypeInfo<DataType::INT_ARRAY>
		{
			using ValueType = std::vector<int32_t>;
			using BaseType = int32_t;
			const vk::Format static BASE_FORMAT = vk::Format::eR32Sint;
		};

		template<>
		struct DataTypeInfo<DataType::VECTOR_2_ARRAY>
		{
			using ValueType = std::vector<Vector2>;
			using BaseType = Vector2;
			const vk::Format static BASE_FORMAT = vk::Format::eR32G32Sfloat;
		};

		template<>
		struct DataTypeInfo<DataType::VECTOR_3_ARRAY>
		{
			using ValueType = std::vector<Vector3>;
			using BaseType = Vector3;
			const vk::Format static BASE_FORMAT = vk::Format::eR32G32B32Sfloat;
		};

		template<>
		struct DataTypeInfo<DataType::VECTOR_4_ARRAY>
		{
			using ValueType = std::vector<Vector4>;
			using BaseType = Vector4;
			const vk::Format static BASE_FORMAT = vk::Format::eR32G32B32A32Sfloat;
		};

		template<>
		struct DataTypeInfo<DataType::COLOR_32_ARRAY>
		{
			using ValueType = std::vector<Color32>;
			using BaseType = Color32;
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

#endif // !VG_MESH_DATA_H
