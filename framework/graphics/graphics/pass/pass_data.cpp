#include "graphics/pass/pass_data.hpp"

namespace vg
{
    PassExtUniformBufferInfo::PassExtUniformBufferInfo(UniformBufferData *pData
        , uint32_t subDataOffset
        , uint32_t subDataCount
        , uint32_t bindingPriority
        )
        : pData(pData)
        , subDataOffset(subDataOffset)
        , subDataCount(subDataCount)
        , bindingPriority(bindingPriority)
    {

    }
        
    PassExtUniformBufferInfo::PassExtUniformBufferInfo(const PassExtUniformBufferInfo &target)
        : pData(target.pData)
        , subDataOffset(target.subDataOffset)
        , subDataCount(target.subDataCount)
        , bindingPriority(target.bindingPriority)
    {

    }
        
    PassExtUniformBufferInfo& PassExtUniformBufferInfo::operator=(const PassExtUniformBufferInfo &target)
    {
        pData = target.pData;
        subDataOffset = target.subDataOffset;
        subDataCount = target.subDataCount;
        bindingPriority = target.bindingPriority;
        return *this;
    }

    PassExtUniformBuffers::PassExtUniformBuffers()
        : arrExtUniformBufferNames()
        , mapExtUniformBuffers()
    {

    }

    const std::vector<std::string> PassExtUniformBuffers::getExtUniformBufferNames() const
    {
        return arrExtUniformBufferNames;
    }

    Bool32 PassExtUniformBuffers::hasExtUniformBuffer(std::string name) const
    {
        return hasValue(name, mapExtUniformBuffers, arrExtUniformBufferNames);
    }
        
    void PassExtUniformBuffers::addExtUniformBuffer(std::string name, const PassExtUniformBufferInfo &extUniformBufferInfo)
    {
        addValue(name, extUniformBufferInfo, mapExtUniformBuffers, arrExtUniformBufferNames);
    }
        
    void PassExtUniformBuffers::removeExtUniformBuffer(std::string name)
    {
        removeValue(name, mapExtUniformBuffers, arrExtUniformBufferNames);
    }
        
    const PassExtUniformBufferInfo &PassExtUniformBuffers::getExtUniformBuffer(std::string name) const
    {
        return getValue(name, mapExtUniformBuffers, arrExtUniformBufferNames);
    }

    void PassExtUniformBuffers::setExtUniformBuffer(std::string name, const PassExtUniformBufferInfo &extUniformBufferInfo)
    {
        setValue(name, extUniformBufferInfo, mapExtUniformBuffers, arrExtUniformBufferNames);
    }
} //vg