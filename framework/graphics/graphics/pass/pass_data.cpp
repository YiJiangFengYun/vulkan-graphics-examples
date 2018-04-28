#include "graphics/pass/pass_data.hpp"

namespace vg
{
	PassData::TexData::TexData(const Texture *pTexture
	    , const Texture::ImageView *pImageView
		, const Texture::Sampler *pSampler
		, vk::ImageLayout imageLayout
		)
		: pTexture(pTexture)
		, pImageView(pImageView)
		, pSampler(pSampler)
		, imageLayout(imageLayout)
	{

	}

	PassData::TexData::TexData(const TexData &target)
	    : pTexture(target.pTexture)
		, pImageView(target.pImageView)
		, pSampler(target.pSampler)
		, imageLayout(target.imageLayout)
	{
		
	}

	PassData::TexData &PassData::TexData::operator=(const TexData &target)
	{
		pTexture = target.pTexture;
		pImageView = target.pImageView;
		pSampler = target.pSampler;
		imageLayout = target.imageLayout;
		return *this;
	}

    PassData::TexData PassData::getTexture(std::string name) const
	{
		return getValue(name, mapTextures);
	}

	void PassData::setTexture(std::string name, TexData texData)
	{
		setValue(name, texData, mapTextures, arrTexNames);
	}

	uint32_t PassData::getDataBaseSize(const std::string name) const
	{
		const auto& bytes = getValue(name, mapDatas);
		const auto& count = getValue(name, mapDataCounts);
		return static_cast<uint32_t>(bytes.size()) / count;
	}

	uint32_t PassData::getDataSize(const std::string name) const
	{
		const auto& bytes = getValue(name, mapDatas);
		return static_cast<uint32_t>(bytes.size());
	}

	void PassData::getDataValue(const std::string name, void *dst, uint32_t size, uint32_t offset) const
	{
		const auto& bytes = getValue(name, mapDatas);		
		if (offset + size > static_cast<uint32_t>(bytes.size()))
		    throw std::range_error("Out range of the saved material data!");
		memcpy(dst, (char *)(bytes.data()) + offset, size);
	}

	void PassData::setDataValue(const std::string name, void *src, uint32_t size, uint32_t offset)
	{
		if (offset + size > static_cast<uint32_t>(mapDatas[name].size()))
		    mapDatas[name].resize(size);
		if(src) memcpy((char *)(mapDatas[name].data()) + offset, src, size);
		auto iterator = std::find(arrDataNames.begin(), arrDataNames.end(), name);
		if (iterator == arrDataNames.end())
		{
			arrDataNames.push_back(name);
		}
		mapDataCounts[name] = 1u;
	}

	void PassData::memoryCopyData(const std::string name
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
} //vg