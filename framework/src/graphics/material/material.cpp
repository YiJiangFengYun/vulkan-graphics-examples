#include "graphics/material/material.hpp"

namespace kgs
{
	Material::Material()
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
		return getValue(name, m_mapColors, m_arrColors);
	}

	void Material::setColor(std::string name, Color value)
	{
		setValue(name, value, m_mapColors, m_arrColors);
	}

	std::vector<Color> Material::getColorArray(std::string name)
	{
		return getValue(name, m_mapColorArrays, m_arrColorArrays);
	}

	void Material::setColorArray(std::string name, std::vector<Color> values)
	{
		setValue(name, values, m_mapColorArrays, m_arrColorArrays);
	}

	float Material::getFloat(std::string name)
	{
		return getValue(name, m_mapFloats, m_arrFloats);
	}

	void Material::setFloat(std::string name, float value)
	{
		setValue(name, value, m_mapFloats, m_arrFloats);
	}

	std::vector<float> Material::getFloatArray(std::string name)
	{
		return getValue(name, m_mapFloatArrays, m_arrFloatArrays);
	}

	void Material::setFloatArray(std::string name, std::vector<float> values)
	{
		setValue(name, values, m_mapFloatArrays, m_arrFloatArrays);
	}

	int32_t Material::getInt(std::string name)
	{
		return getValue(name, m_mapInts, m_arrInts);
	}

	void Material::setInt(std::string name, int32_t value)
	{
		setValue(name, value, m_mapInts, m_arrInts);
	}

	std::vector<int32_t> Material::getIntArray(std::string name)
	{
		return getValue(name, m_mapIntArrays, m_arrIntArrays);
	}

	void Material::setIntArray(std::string name, std::vector<int32_t> values)
	{
		setValue(name, values, m_mapIntArrays, m_arrIntArrays);
	}

	Vector4 Material::getVector(std::string name)
	{
		return getValue(name, m_mapVectors, m_arrVectors);
	}

	void Material::setVector(std::string name, Vector4 value)
	{
		setValue(name, value, m_mapVectors, m_arrVectors);
	}

	std::vector<Vector4> Material::getVectorArray(std::string name)
	{
		return getValue(name, m_mapVectorArrays, m_arrVectorArrays);
	}

	void Material::setVectorArray(std::string name, std::vector<Vector4> values)
	{
		setValue(name, values, m_mapVectorArrays, m_arrVectorArrays);
	}

	Matrix4x4 Material::getMatrix(std::string name)
	{
		return getValue(name, m_mapMatrixs, m_arrMatrixs);
	}

	void Material::setMatrix(std::string name, Matrix4x4 value)
	{
		setValue(name, value, m_mapMatrixs, m_arrMatrixs);
	}

	std::vector<Matrix4x4> Material::getMatrixArray(std::string name)
	{
		return getValue(name, m_mapMatrixArrays, m_arrMatrixArrays);
	}

	void Material::setMatrixArray(std::string name, std::vector<Matrix4x4> values)
	{
		setValue(name, values, m_mapMatrixArrays, m_arrMatrixArrays);
	}

	std::shared_ptr<Texture> Material::getTexture(std::string name)
	{
		return getValue(name, m_mapTextures, m_arrTextures);
	}

	void Material::setTexture(std::string name, std::shared_ptr<Texture> value)
	{
		setValue(name, value, m_mapTextures, m_arrTextures);
	}

	Vector2 Material::getTextureOffset(std::string name)
	{
		return getValue(name, m_mapTextureOffsets, m_arrTextureOffsets);
	}

	void Material::setTextureOffset(std::string name, Vector2 value)
	{
		setValue(name, value, m_mapTextureOffsets, m_arrTextureOffsets);
	}

	Vector2 Material::getTextureScale(std::string name)
	{
		return getValue(name, m_mapTextureScales, m_arrTextureScales);
	}

	void Material::setTextureScale(std::string name, Vector2 value)
	{
		setValue(name, value, m_mapTextureScales, m_arrTextureScales);
	}
}