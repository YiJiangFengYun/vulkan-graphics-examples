#include "graphics/material/material.hpp"

namespace kgs
{
	Material::Material():
		m_data(new MaterialData())
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

	Color Material::getColor(std::string name)
	{
		return getValue(name, m_data->mapColors, m_data->arrColors);
	}

	void Material::setColor(std::string name, Color value)
	{
		setValue(name, value, m_data->mapColors, m_data->arrColors);
	}

	std::vector<Color> Material::getColorArray(std::string name)
	{
		return getValue(name, m_data->mapColorArrays, m_data->arrColorArrays);
	}

	void Material::setColorArray(std::string name, std::vector<Color> values)
	{
		setValue(name, values, m_data->mapColorArrays, m_data->arrColorArrays);
	}

	float Material::getFloat(std::string name)
	{
		return getValue(name, m_data->mapFloats, m_data->arrFloats);
	}

	void Material::setFloat(std::string name, float value)
	{
		setValue(name, value, m_data->mapFloats, m_data->arrFloats);
	}

	std::vector<float> Material::getFloatArray(std::string name)
	{
		return getValue(name, m_data->mapFloatArrays, m_data->arrFloatArrays);
	}

	void Material::setFloatArray(std::string name, std::vector<float> values)
	{
		setValue(name, values, m_data->mapFloatArrays, m_data->arrFloatArrays);
	}

	int32_t Material::getInt(std::string name)
	{
		return getValue(name, m_data->mapInts, m_data->arrInts);
	}

	void Material::setInt(std::string name, int32_t value)
	{
		setValue(name, value, m_data->mapInts, m_data->arrInts);
	}

	std::vector<int32_t> Material::getIntArray(std::string name)
	{
		return getValue(name, m_data->mapIntArrays, m_data->arrIntArrays);
	}

	void Material::setIntArray(std::string name, std::vector<int32_t> values)
	{
		setValue(name, values, m_data->mapIntArrays, m_data->arrIntArrays);
	}

	Vector4 Material::getVector(std::string name)
	{
		return getValue(name, m_data->mapVectors, m_data->arrVectors);
	}

	void Material::setVector(std::string name, Vector4 value)
	{
		setValue(name, value, m_data->mapVectors, m_data->arrVectors);
	}

	std::vector<Vector4> Material::getVectorArray(std::string name)
	{
		return getValue(name, m_data->mapVectorArrays, m_data->arrVectorArrays);
	}

	void Material::setVectorArray(std::string name, std::vector<Vector4> values)
	{
		setValue(name, values, m_data->mapVectorArrays, m_data->arrVectorArrays);
	}

	Matrix4x4 Material::getMatrix(std::string name)
	{
		return getValue(name, m_data->mapMatrixs, m_data->arrMatrixs);
	}

	void Material::setMatrix(std::string name, Matrix4x4 value)
	{
		setValue(name, value, m_data->mapMatrixs, m_data->arrMatrixs);
	}

	std::vector<Matrix4x4> Material::getMatrixArray(std::string name)
	{
		return getValue(name, m_data->mapMatrixArrays, m_data->arrMatrixArrays);
	}

	void Material::setMatrixArray(std::string name, std::vector<Matrix4x4> values)
	{
		setValue(name, values, m_data->mapMatrixArrays, m_data->arrMatrixArrays);
	}

	std::shared_ptr<Texture> Material::getTexture(std::string name)
	{
		return getValue(name, m_data->mapTextures, m_data->arrTextures);
	}

	void Material::setTexture(std::string name, std::shared_ptr<Texture> value)
	{
		setValue(name, value, m_data->mapTextures, m_data->arrTextures);
	}

	Vector2 Material::getTextureOffset(std::string name)
	{
		return getValue(name, m_data->mapTextureOffsets, m_data->arrTextureOffsets);
	}

	void Material::setTextureOffset(std::string name, Vector2 value)
	{
		setValue(name, value, m_data->mapTextureOffsets, m_data->arrTextureOffsets);
	}

	Vector2 Material::getTextureScale(std::string name)
	{
		return getValue(name, m_data->mapTextureScales, m_data->arrTextureScales);
	}

	void Material::setTextureScale(std::string name, Vector2 value)
	{
		setValue(name, value, m_data->mapTextureScales, m_data->arrTextureScales);
	}
}