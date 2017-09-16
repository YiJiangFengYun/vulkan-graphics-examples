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

		template <MaterialData::DataType dataType>
		typename MaterialData::ValueTypeInfo<dataType>::value_t getData(std::string name)
		{
			return m_pData->getDataValue<dataType>(name);
		}

		template <MaterialData::DataType dataType>
		void setData(std::string name, typename MaterialData::ValueTypeInfo<dataType>::value_t value)
		{
			m_pData->setDataValue<dataType>(name, value);
		}

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
