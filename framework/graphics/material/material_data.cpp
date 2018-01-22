#include "graphics/material/material_data.hpp"

namespace vg
{
	MaterialData::BuildInData::BuildInData()
		: matrixObjectToNDC(1.0f)
		, mainColor(1.0f, 1.0f, 1.0f, 1.0f)
		, matrixObjectToView(1.0f)
		, matrixObjectToWorld(1.0f)
	{

	}

	MaterialData::BuildInData::BuildInData(Matrix4x4 matrixObjectToNDC
		, Color mainColor
		, Matrix4x4 matrixObjectToView
		, Matrix4x4 matrixObjectToWorld)
		: matrixObjectToNDC(matrixObjectToNDC)
		, mainColor(mainColor)
		, matrixObjectToView(matrixObjectToView)
		, matrixObjectToWorld(matrixObjectToWorld)
	{

	}

	MaterialData::BuildInData::BuildInData(const BuildInData &target)
		: matrixObjectToNDC(target.matrixObjectToNDC)
		, mainColor(target.mainColor)
		, matrixObjectToView(target.matrixObjectToView)
		, matrixObjectToWorld(target.matrixObjectToWorld)
	{

	}

	MaterialData::BuildInData::BuildInData(const BuildInData &&target)
		: matrixObjectToNDC(target.matrixObjectToNDC)
		, mainColor(target.mainColor)
		, matrixObjectToView(target.matrixObjectToView)
		, matrixObjectToWorld(target.matrixObjectToWorld)
	{

	}

	const std::shared_ptr<Texture> &MaterialData::getTexture(std::string name) const
	{
		return getValue(name, mapTextures);
	}

	void MaterialData::setTexture(std::string name, const std::shared_ptr<Texture> &pTex)
	{
		setValue(name, pTex, mapTextures, arrTexNames);
	}

	uint32_t MaterialData::getDataBaseSize(std::string name) const
	{
		const auto& bytes = getValue(name, mapDatas);
		const auto& count = getValue(name, mapDataCounts);
		return static_cast<uint32_t>(bytes.size()) / count;
	}

	uint32_t MaterialData::getDataSize(std::string name) const
	{
		const auto& bytes = getValue(name, mapDatas);
		return static_cast<uint32_t>(bytes.size());
	}

	void MaterialData::memoryCopyData(std::string name
		, void* dst
		, uint32_t offset
		, uint32_t elementStart
		, uint32_t maxElementCount) const
	{
		const auto& bytes = getValue(name, mapDatas);
		const auto& count = getValue(name, mapDataCounts);
		uint32_t baseSize = static_cast<uint32_t>(bytes.size()) / count;
		char *ptr = static_cast<char *>(dst);
		ptr += offset;
		uint32_t finalElementCount = std::max(0u, std::min(count - elementStart, maxElementCount));
		if (finalElementCount == 0) return;
		uint32_t srcOffset = elementStart * baseSize;
		uint32_t srcSize = finalElementCount * baseSize;
		std::memcpy(ptr, bytes.data() + srcOffset, srcSize);
	}
}

