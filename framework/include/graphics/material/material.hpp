#ifndef KGS_MATERIAL_H
#define KGS_MATERIAL_H

#include <memory>
#include <vector>
#include <unordered_map>
#include "graphics/global.hpp"
#include "graphics/material/pass.hpp"
#include "graphics/texture/texture.hpp"

namespace kgs
{
	class Material
	{
	public:
		Material();
		~Material();

		int32_t getPassIndex(std::string name);
		std::string getPassName(int32_t index);
		std::shared_ptr<Pass> getPass(std::string name);
		void setPass(std::string name, std::shared_ptr<Pass> pass);

		Color getColor(std::string name);
		void setColor(std::string name, Color value);
		std::vector<Color> getColorArray(std::string name);
		void setColorArray(std::string name, std::vector<Color> values);
		
		float getFloat(std::string name);
		void setFloat(std::string name, float value);
		std::vector<float> getFloatArray(std::string name);
		void setFloatArray(std::string name, std::vector<float> values);

		int32_t getInt(std::string name);
		void setInt(std::string name, int32_t value);
		std::vector<int32_t> getIntArray(std::string name);
		void setIntArray(std::string name, std::vector<int32_t> values);

		Vector4 getVector(std::string name);
		void setVector(std::string name, Vector4 value);
		std::vector<Vector4> getVectorArray(std::string name);
		void setVectorArray(std::string name, std::vector<Vector4> values);

		Matrix4x4 getMatrix(std::string name);
		void setMatrix(std::string name, Matrix4x4 value);
		std::vector<Matrix4x4> getMatrixArray(std::string name);
		void setMatrixArray(std::string name, std::vector<Matrix4x4> values);

		std::shared_ptr<Texture> getTexture(std::string name);
		void setTexture(std::string name, std::shared_ptr<Texture> value);
		Vector2 getTextureOffset(std::string name);
		void setTextureOffset(std::string name, Vector2 value);
		Vector2 getTextureScale(std::string name);
		void setTextureScale(std::string name, Vector2 value);

	private:
		//--compositions
		uint32_t m_renderQueue;
		Color m_mainColor;
		Vector2 m_mainTextureOffset;
		Vector2 m_mainTextureScale;
		//--compositions

		//--aggregations
		std::vector<std::shared_ptr<Pass>> m_arrPasses;
		std::unordered_map<std::string, std::shared_ptr<Pass>> m_mapPasses;

		std::vector<Color> m_arrColors;
		std::unordered_map<std::string, Color> m_mapColors;
		std::vector<std::vector<Color>> m_arrColorArrays;
		std::unordered_map<std::string, std::vector<Color>> m_mapColorArrays;

		std::vector<float> m_arrFloats;
		std::unordered_map<std::string, float> m_mapFloats;
		std::vector<std::vector<float>> m_arrFloatArrays;
		std::unordered_map<std::string, std::vector<float>> m_mapFloatArrays;

		std::vector<int32_t> m_arrInts;
		std::unordered_map<std::string, int32_t> m_mapInts;
		std::vector<std::vector<int32_t>> m_arrIntArrays;
		std::unordered_map<std::string, std::vector<int32_t>> m_mapIntArrays;

		std::vector<Vector4> m_arrVectors;
		std::unordered_map<std::string, Vector4> m_mapVectors;
		std::vector<std::vector<Vector4>> m_arrVectorArrays;
		std::unordered_map<std::string, std::vector<Vector4>> m_mapVectorArrays;

		std::vector<Matrix4x4> m_arrMatrixs;
		std::unordered_map<std::string, Matrix4x4> m_mapMatrixs;
		std::vector<std::vector<Matrix4x4>> m_arrMatrixArrays;
		std::unordered_map<std::string, std::vector<Matrix4x4>> m_mapMatrixArrays;

		std::shared_ptr<Texture> m_mainTexture;
		std::vector<std::shared_ptr<Texture>> m_arrTextures;
		std::unordered_map<std::string, std::shared_ptr<Texture>> m_mapTextures;
		std::vector<Vector2> m_arrTextureOffsets;
		std::unordered_map<std::string, Vector2> m_mapTextureOffsets;
		std::vector<Vector2> m_arrTextureScales;
		std::unordered_map<std::string, Vector2> m_mapTextureScales;
		//--aggregations

		//tool methods
		template <typename T>
		inline T getValue(std::string key, std::unordered_map<std::string, T> map, std::vector<T> arr)
		{
			auto iterator = map.find(name);
			if (iterator == map.end())
			{
				return {};
			}
			else
			{
				return iterator->second;
			}
		}

		template <typename T>
		inline void setValue(std::string key, T value, std::unordered_map<std::string, T> map, std::vector<T> arr)
		{
			auto iterator = map.find(name);
			if (iterator == map.end())
			{
				map.insert(std::make_pair(name, value));
				arr.push_back(value);
			}
			else
			{
				iterator->second = value;
				auto iterator2 = std::find(arr.begin(), arr.end(), iterator->second);
				*iterator2 = value;
			}
		}
	};
}

#endif // !KGS_MATERIAL_H
