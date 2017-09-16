#include "graphics/material/material.hpp"

namespace kgs
{
	Material::Material() :
		m_pData(new MaterialData())
	{
	}

	Material::~Material()
	{
	}

	int32_t Material::getPassIndex(std::string name)
	{
		auto iterator = m_mapPasses.find(name);
		if (iterator == m_mapPasses.end()) return -1;
		//linear complexity
		auto iterator2 = std::find(m_arrPasses.cbegin(), m_arrPasses.cend(), iterator->second);
		return static_cast<int32_t>(iterator2 - m_arrPasses.cbegin());

	}

	std::string Material::getPassName(int32_t index)
	{
		auto pass = *(m_arrPasses.cbegin() + index);
		//linear complexity
		auto pos = std::find_if(m_mapPasses.cbegin(), m_mapPasses.cend(), [pass](const std::pair<std::string, std::shared_ptr<Pass>>& elem) {
			return elem.second == pass;
		});
		return pos->first;
	}

	std::shared_ptr<Pass> Material::getPass(std::string name)
	{
		return getValue(name, m_mapPasses, m_arrPasses);
	}

	void Material::setPass(std::string name, std::shared_ptr<Pass> pass)
	{
		setValue(name, pass, m_mapPasses, m_arrPasses);
	}

	std::shared_ptr<Texture> Material::getMainTexture()
	{
		return getData<MaterialData::DataType::TEXTURE>(MainTextureName);
	}

	void Material::setMainTexture(std::shared_ptr<Texture> value)
	{
		setData<MaterialData::DataType::TEXTURE>(MainTextureName, value);
	}

	Vector2 Material::getMainTextureOffset()
	{
		return getData<MaterialData::DataType::TEXTURE_OFFSET>(MainTextureName);
	}

	void Material::setMainTextureOffset(Vector2 value)
	{
		setData<MaterialData::DataType::TEXTURE_OFFSET>(MainTextureName, value);
	}

	Vector2 Material::getMainTextureScale()
	{
		return getData<MaterialData::DataType::TEXTURE_SCALE>(MainTextureName);
	}

	void Material::setMainTextureScale(Vector2 value)
	{
		setData<MaterialData::DataType::TEXTURE_SCALE>(MainTextureName, value);
	}

	Color Material::getMainColor()
	{
		return getData<MaterialData::DataType::COLOR>(MainColorName);
	}
	
	void Material::setMainColor(Color value)
	{
		setData<MaterialData::DataType::COLOR>(MainColorName, value);
	}

	void Material::apply()
	{
		for (const auto& item : m_arrPasses)
		{
			item->apply();
		}
	}
}