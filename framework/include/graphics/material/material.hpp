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
		void setIntArray(std::string anme, std::vector<int32_t> values);

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
		Color m_mainColor;
		Vector2 m_mainTextureOffset;
		Vector2 m_mainTextureScale;
		uint32_t m_renderQueue;
		//--compositions

		//--aggregations
		std::vector<std::shared_ptr<Pass>> m_arrPasses;
		std::unordered_map<std::string, std::shared_ptr<Pass>> m_mapPasses;

		std::shared_ptr<Texture> m_mainTexture;
		std::vector<std::shared_ptr<Texture>> m_arrTextures;
		std::unordered_map<std::string, std::shared_ptr<Texture>> m_mapTextures;
		//--aggregations
	};
}

#endif // !KGS_MATERIAL_H
