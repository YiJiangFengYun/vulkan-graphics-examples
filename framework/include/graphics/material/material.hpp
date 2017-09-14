#ifndef KGS_MATERIAL_H
#define KGS_MATERIAL_H

#include <memory>
#include "graphics/global.hpp"
#include "graphics/util/util.hpp"
#include "graphics/material/pass.hpp"
#include "graphics/texture/texture.hpp"
#include "graphics/material/material_data.hpp"

#define MainColorName "_Color"
#define MainTextureName "_MainTex"

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

		float getFloat(std::string name);
		void setFloat(std::string name, float value);
		std::vector<float> getFloatArray(std::string name);
		void setFloatArray(std::string name, std::vector<float> values);

		int32_t getInt(std::string name);
		void setInt(std::string name, int32_t value);
		std::vector<int32_t> getIntArray(std::string name);
		void setIntArray(std::string name, std::vector<int32_t> values);

		Color getColor(std::string name);
		void setColor(std::string name, Color value);
		std::vector<Color> getColorArray(std::string name);
		void setColorArray(std::string name, std::vector<Color> values);

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

		std::shared_ptr<Texture> getMainTexture();
		void setMainTexture(std::shared_ptr<Texture> value);
		Vector2 getMainTextureOffset();
		void setMainTextureOffset(Vector2 value);
		Vector2 getMainTextureScale();
		void setMainTextureScale(Vector2 value);

		Color getMainColor();
		void setMainColor(Color value);

		void apply();

	private:
		//--compositions
		uint32_t m_renderQueue;
		std::shared_ptr<MaterialData> m_pData;
		//--compositions

		//--aggregations
		std::vector<std::shared_ptr<Pass>> m_arrPasses;
		std::unordered_map<std::string, std::shared_ptr<Pass>> m_mapPasses;
		//--aggregations

		//tool methods
		
	};
}

#endif // !KGS_MATERIAL_H