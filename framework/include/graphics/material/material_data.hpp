#ifndef KGS_MATERIAL_DATA_H
#define KGS_MATERIAL_DATA_H

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
		class ValueTypeOfType
		{
			typedef void value_t;
		};


		template<>
		class ValueTypeOfType<DataType::FLOAT>
		{
			typedef float value_t;
		};

		template<>
		class ValueTypeOfType<DataType::FLOAT_ARRAY>
		{
			typedef std::vector<float> value_t;
		};

		template<>
		class ValueTypeOfType<DataType::INT>
		{
			typedef uint32_t value_t;
		};

		template<>
		class ValueTypeOfType<DataType::INT_ARRAY>
		{
			typedef std::vector<uint32_t> value_t;
		};

		template<>
		class ValueTypeOfType<DataType::COLOR>
		{
			typedef Color value_t;
		};

		template<>
		class ValueTypeOfType<DataType::COLOR_ARRAY>
		{
			typedef std::vector<Color> value_t;
		};

		template<>
		class ValueTypeOfType<DataType::VECTOR>
		{
			typedef Vector4 value_t;
		};

		template<>
		class ValueTypeOfType<DataType::VECTOR_ARRAY>
		{
			typedef std::vector<Vector4> value_t;
		};

		template<>
		class ValueTypeOfType<DataType::MATRIX>
		{
			typedef Matrix4x4 value_t;
		};

		template<>
		class ValueTypeOfType<DataType::MATRIX_ARRAY>
		{
			typedef std::vector<Matrix4x4> value_t;
		};

		template<>
		class ValueTypeOfType<DataType::TEXTURE>
		{
			typedef Texture value_t;
		};

		template<>
		class ValueTypeOfType<DataType::TEXTURE_OFFSET>
		{
			typedef Vector2 value_t;
		};

		template<>
		class ValueTypeOfType<DataType::TEXTURE_SCALE>
		{
			typedef Vector2 value_t;
		};


		uint32_t getDataValueSize(std::string name, DataType type);

		template <DataType type, typename T = ValueTypeOfType<type>::value_t>
		T getDataValue(std::string name) {}

		template<>
		float getDataValue<DataType::FLOAT>(std::string name);

		template<>
		std::vector<float> getDataValue<DataType::FLOAT_ARRAY>(std::string name);

		template<>
		int getDataValue<DataType::INT>(std::string name);

		template<>
		std::vector<int> getDataValue<DataType::INT_ARRAY>(std::string name);

		template<>
		Color getDataValue<DataType::COLOR>(std::string name);

		template<>
		std::vector<Color> getDataValue<DataType::COLOR_ARRAY>(std::string name);

		template<>
		Vector4 getDataValue<DataType::VECTOR>(std::string name);

		template<>
		std::vector<Vector4> getDataValue<DataType::VECTOR_ARRAY>(std::string name);

		template<>
		Matrix4x4 getDataValue<DataType::MATRIX>(std::string name);

		template<>
		std::vector<Matrix4x4> getDataValue<DataType::MATRIX_ARRAY>(std::string name);

		template<>
		std::shared_ptr<Texture> getDataValue<DataType::TEXTURE>(std::string name);

		template<>
		Vector2 getDataValue<DataType::TEXTURE_OFFSET>(std::string name);

		template<>
		Vector2 getDataValue<DataType::TEXTURE_SCALE>(std::string name);
	};
} //namespace kgs

#endif // !KGS_MATERIAL_DATA_H
