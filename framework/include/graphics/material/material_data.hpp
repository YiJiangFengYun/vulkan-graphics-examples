#ifndef KGS_MATERIAL_DATA_H
#define KGS_MATERIAL_DATA_H

#include <vector>
#include <unordered_map>
#include "graphics/global.hpp"
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

		std::vector<void*> arrTypeToMap;

		MaterialData():
			arrTypeToMap({
			&mapFloats,
			&mapFloatArrays,
			&mapInts,
			&mapIntArrays,
			&mapColors,
			&mapColorArrays,
			&mapVectors,
			&mapVectorArrays,
			&mapMatrixs,
			&mapMatrixArrays,
			&mapTextures,
			&mapTextureOffsets,
			&mapTextureScales
		})
		{
#ifdef DEBUG
			if (static_cast<uint32_t>(arrTypeToMap.size()) != static_cast<uint32_t>(DataType::RANGE_SIZE))
				throw std::length_error("Map count is not equal to data type count.");
#endif // DEBUG

		}
	};
} //namespace kgs

#endif // !KGS_MATERIAL_DATA_H
