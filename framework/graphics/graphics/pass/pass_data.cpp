#include "graphics/pass/pass_data.hpp"

namespace vg
{
	PassDataInfo::PassDataInfo(uint32_t layoutPriority
		, vk::ShaderStageFlags shaderStageFlags
	    )
		: layoutPriority(layoutPriority)
		, shaderStageFlags(shaderStageFlags)
	{
		
	}

	PassDataInfo::PassDataInfo(const PassDataInfo &target)
	    : layoutPriority(target.layoutPriority)
		, shaderStageFlags(target.shaderStageFlags)
	{

	}

	PassDataInfo& PassDataInfo::operator=(const PassDataInfo &target)
	{
		layoutPriority = target.layoutPriority;
		shaderStageFlags = target.shaderStageFlags;
		return *this;
	}

	PassDataSizeInfo::PassDataSizeInfo(uint32_t size
	    )
		: size(size)
	{
		
	}

	PassDataSizeInfo::PassDataSizeInfo(const PassDataSizeInfo &target)
	    : size(target.size)
	{

	}

	PassDataSizeInfo& PassDataSizeInfo::operator=(const PassDataSizeInfo &target)
	{
		size = target.size;
		return *this;
	}

	uint32_t PassDataSizeInfo::getBufferSize() const
	{
		const auto &pPhysicalDevice = pApp->getPhysicalDevice();
		const auto &properties = pPhysicalDevice->getProperties();
		const auto &minOffsetAlignment = static_cast<float>(properties.limits.minUniformBufferOffsetAlignment);
		return static_cast<uint32_t>(std::ceil(size / minOffsetAlignment) * minOffsetAlignment);
	}

	PassTextureInfo::PassTextureInfo(const Texture *pTexture
	    , const Texture::ImageView *pImageView
		, const Texture::Sampler *pSampler
		, vk::ImageLayout imageLayout
		, uint32_t bindingPriority
		, ImageDescriptorType descriptorType
		, vk::ShaderStageFlags stageFlags
	)
	    : pTexture(pTexture)
		, pImageView(pImageView)
		, pSampler(pSampler)
		, imageLayout(imageLayout)
		, bindingPriority(bindingPriority)
		, descriptorType(descriptorType)
		, stageFlags(stageFlags)
	{

	}

	PassTextureInfo::PassTextureInfo(const PassTextureInfo &target)
	    : pTexture(target.pTexture)
		, pImageView(target.pImageView)
		, pSampler(target.pSampler)
		, imageLayout(target.imageLayout)
		, bindingPriority(target.bindingPriority)
		, descriptorType(target.descriptorType)
		, stageFlags(target.stageFlags)
	{

	}

	PassTextureInfo& PassTextureInfo::operator=(const PassTextureInfo &target)
	{
		pTexture = target.pTexture;
		pImageView = target.pImageView;
		pSampler = target.pSampler;
		imageLayout = target.imageLayout;
		bindingPriority = target.bindingPriority;
		descriptorType = target.descriptorType;
		stageFlags = target.stageFlags;
		return *this;
	}


	PassBufferInfo::PassBufferInfo(const BufferData *pBuffer
		, uint32_t offset
		, uint32_t range
		, uint32_t bindingPriority
		, BufferDescriptorType descriptorType
		, vk::ShaderStageFlags stageFlags
		)
		: pBuffer(pBuffer)
		, offset(offset)
		, range(range)
		, bindingPriority(bindingPriority)
		, descriptorType(descriptorType)
		, stageFlags(stageFlags)
	{
		
	}

	PassBufferInfo::PassBufferInfo(const PassBufferInfo &target)
	    : pBuffer(target.pBuffer)
		, offset(target.offset)
		, range(target.range)
		, bindingPriority(target.bindingPriority)
		, descriptorType(target.descriptorType)
		, stageFlags(target.stageFlags)
	{

	}

	PassBufferInfo& PassBufferInfo::operator=(const PassBufferInfo &target)
	{
		pBuffer = target.pBuffer;
		offset = target.offset;
		range = target.range;
		bindingPriority = target.bindingPriority;
		descriptorType = target.descriptorType;
		stageFlags = target.stageFlags;
		return *this;
	}

	PassData::PassData()
	   : arrDataNames()
	   , mapDatas()
	   , mapDataCounts()
	   , mapDataInfos()
	   , mapDataSizeInfos()
	   , arrBufferNames()
	   , mapBuffers()
	   , arrTexNames()
	   , mapTextures()
	{

	}

	const std::vector<std::string> PassData::getArrBufferNames() const
	{
		return arrBufferNames;
	}

	Bool32 PassData::hasBuffer(std::string name) const
	{
		return hasValue<PassBufferInfo>(name, mapBuffers);
	}

	void PassData::addBuffer(std::string name, const PassBufferInfo &bufferInfo)
	{
		addValue(name, bufferInfo, mapBuffers, arrBufferNames);
	}

	void PassData::removeBuffer(std::string name)
	{
		removeValue(name, mapBuffers, arrBufferNames);
	}

	const PassBufferInfo &PassData::getBuffer(std::string name) const
	{
		return getValue(name, mapBuffers, arrBufferNames);
	}

	void PassData::setBuffer(std::string name, const PassBufferInfo &bufferInfo)
	{
		setValue(name, bufferInfo, mapBuffers, arrBufferNames);
	}

	const std::vector<std::string> PassData::getArrTextureNames() const
	{
		return arrTexNames;
	}

	Bool32 PassData::hasTexture(std::string name) const
	{
		return hasValue<PassTextureInfo>(name, mapTextures);
	}
		
	void PassData::addTexture(std::string name, const PassTextureInfo &texInfo)
	{
		addValue(name, texInfo, mapTextures, arrTexNames);
	}
		
	void PassData::removeTexture(std::string name)
	{
		removeValue(name, mapTextures, arrTexNames);
	}

    const PassTextureInfo &PassData::getTexture(std::string name) const
	{
		return getValue(name, mapTextures, arrTexNames);
	}

	void PassData::setTexture(std::string name, const PassTextureInfo &texInfo)
	{
		setValue(name, texInfo, mapTextures, arrTexNames);
	}

	const std::vector<std::string> PassData::getArrDataNames() const
	{
		return arrDataNames;
	}

	Bool32 PassData::hasData(std::string name) const
	{
		return hasValue<std::vector<Byte>>(name, mapDatas);
	}
		
	void PassData::removeData(std::string name)
	{
		removeValue(name, mapDatas, arrDataNames);
		removeValue(name, mapDataCounts);
		removeValue(name, mapDataInfos);
		removeValue(name, mapDataSizeInfos);
	}

	const PassDataInfo &PassData::getDataInfo(std::string name) const
	{
		return getValue(name, mapDataInfos);
	}

	const PassDataSizeInfo &PassData::getDataSizeInfo(std::string name) const
	{
		return getValue(name, mapDataSizeInfos);
	}

	void PassData::addData(const std::string name, const PassDataInfo &info, const PassDataSizeInfo &sizeInfo)
	{
		std::vector<Byte> temp(sizeInfo.size);
		addValue(name, temp, mapDatas, arrDataNames);
		addValue(name, 1u, mapDataCounts);
		addValue(name, info, mapDataInfos);
		addValue(name, sizeInfo, mapDataSizeInfos);
	}

	void PassData::addData(const std::string name, const PassDataInfo &info, void *src, uint32_t size)
	{
		std::vector<Byte> temp(size);
		if (size != 0u && src != nullptr) memcpy(temp.data(), src, size);
		addValue(name, temp, mapDatas, arrDataNames);
		addValue(name, 1u, mapDataCounts);
		addValue(name, info, mapDataInfos);
		PassDataSizeInfo sizeInfo = {
			size
		};
		addValue(name, sizeInfo, mapDataSizeInfos);
	}

	void PassData::getData(const std::string name, void *dst, uint32_t size, uint32_t offset) const
	{
		const auto &bytes = getValue(name, mapDatas);		
		if (offset + size > static_cast<uint32_t>(bytes.size()))
		    throw std::range_error("Out range of the saved material data!");
		memcpy(dst, (char *)(bytes.data()) + offset, size);
	}

	void PassData::setData(const std::string name, void *src, uint32_t size, uint32_t offset)
	{
		const auto& iterator = mapDatas.find(name);
		if (iterator == mapDatas.cend())
		{
			throw std::runtime_error("Map don't has item whose key: " + name);
		}
		else
		{
			if (offset + size > static_cast<uint32_t>(mapDatas[name].size()))
		        mapDatas[name].resize(offset + size);
		    if(src) memcpy((char *)(mapDatas[name].data()) + offset, src, size);
			setValue(name, 1u, mapDataCounts);
			PassDataSizeInfo sizeInfo = {
		    	size
		    };
		    setValue(name, sizeInfo, mapDataSizeInfos);
		}
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