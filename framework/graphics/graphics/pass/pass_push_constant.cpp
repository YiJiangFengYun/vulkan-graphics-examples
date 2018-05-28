#include "graphics/pass/pass_push_constant.hpp"

#include "graphics/util/util.hpp"

namespace vg
{
    PassPushConstantData::ConstantRange::ConstantRange(uint32_t size
		// , uint32_t offset
		)
        : size(size)
    {}

    PassPushConstantData::ConstantRange::ConstantRange(const ConstantRange &target)
        : size(target.size)
    {

    }
	
    PassPushConstantData::ConstantRange& PassPushConstantData::ConstantRange::operator=(const ConstantRange &target)
    {
        size = target.size;
		return *this;
    }

    PassPushConstantData::ConstantUpdate::ConstantUpdate(uint32_t offset
		, uint32_t size
		, std::vector<Byte> data
		)
        : m_offset(offset)
        , m_size(0u)
        , m_data()
    {
        setData(data.data(), size);
    }

    PassPushConstantData::ConstantUpdate::ConstantUpdate(const ConstantUpdate &target)
        : m_offset(target.m_offset)
        , m_size(target.m_size)
        , m_data(target.m_data)
    {

    }
		    
    PassPushConstantData::ConstantUpdate& PassPushConstantData::ConstantUpdate::operator=(const ConstantUpdate &target)
    {
        m_offset = target.m_offset;
        m_size = target.m_size;
        m_data = target.m_data;
		return *this;
    }

    void PassPushConstantData::ConstantUpdate::set(uint32_t offset
		, const void *pData
		, uint32_t size
		)
    {
        m_offset = offset;
        setData(pData, size);
    }

    void PassPushConstantData::ConstantUpdate::setData(const void *pData
		, uint32_t size
		)
    {
        if (m_data.size() < size) {
            m_data.resize(size);
        }
        memcpy(m_data.data(), pData, size);
		m_size = size;
    }

    uint32_t PassPushConstantData::ConstantUpdate::getOffset() const
    {
        return m_offset;
    }

	const void *PassPushConstantData::ConstantUpdate::getData() const
    {
        return m_data.data();
    }
			
    uint32_t PassPushConstantData::ConstantUpdate::getSize() const
    {
        return m_size;
    }

    PassPushConstantData::ConstantItem::ConstantItem(uint32_t priority
		, vk::ShaderStageFlags stageFlags
		, uint32_t size
		)
        : m_priority(priority)
        , m_stageFlags(stageFlags)
        , m_range(size)
        , m_update()
    {

    }

    PassPushConstantData::ConstantItem::ConstantItem(const ConstantItem &target)
        : m_priority(target.m_priority)
        , m_stageFlags(target.m_stageFlags)
        , m_range(target.m_range)
        , m_update(target.m_update)
    {

    }
		
    PassPushConstantData::ConstantItem &PassPushConstantData::ConstantItem::operator=(const ConstantItem &target)
    {
        m_priority = target.m_priority;
        m_stageFlags = target.m_stageFlags;
        m_range = target.m_range;
        m_update = target.m_update;
		return *this;
    }

    uint32_t PassPushConstantData::ConstantItem::getPriority() const
    {
        return m_priority;
    }

    vk::ShaderStageFlags PassPushConstantData::ConstantItem::getStageFlags() const
    {
        return m_stageFlags;
    }

	const PassPushConstantData::ConstantRange &PassPushConstantData::ConstantItem::getRange() const
    {
        return m_range;
    }
	
    PassPushConstantData::ConstantRange &PassPushConstantData::ConstantItem::getRange()
    {
        return m_range;
    }
	
    const PassPushConstantData::ConstantUpdate &PassPushConstantData::ConstantItem::getUpdate() const
    {
        return m_update;
    }
			
    PassPushConstantData::ConstantUpdate &PassPushConstantData::ConstantItem::getUpdate()
    {
        return m_update;
    }

    PassPushConstantData::PassPushConstantData()
        : m_mapItems()
        , m_arrItemNames()
    {

    }

    Bool32 PassPushConstantData::hasPushConstant(std::string name) const
    {
        return hasValue(name, m_mapItems, m_arrItemNames);
    }

    void PassPushConstantData::addPushConstant(std::string name
        , uint32_t priority
		, vk::ShaderStageFlags stageFlags		
		, uint32_t size
		)
    {
        ConstantItem temp(priority, stageFlags, size);
        addValue(name, temp, m_mapItems, m_arrItemNames);
    }

    void PassPushConstantData::removePushConstant(std::string name)
    {
        removeValue(name, m_mapItems, m_arrItemNames);
    }

    const PassPushConstantData::ConstantItem &PassPushConstantData::getPushConstant(std::string name) const
    {
        return getValue(name, m_mapItems, m_arrItemNames);
    }

    void PassPushConstantData::setPushConstant(std::string name
        , uint32_t priority
		, vk::ShaderStageFlags stageFlags		
		, uint32_t size
		)
    {
        ConstantItem temp(priority, stageFlags, size);
        setValue(name, temp, m_mapItems, m_arrItemNames);
    }

    void PassPushConstantData::setPushConstantUpdate(std::string name
		, uint32_t offset
		, const void *pData
		, uint32_t size
		)
    {
        auto &temp = getValue(name, m_mapItems, m_arrItemNames);
        temp.getUpdate().set(offset, pData, size);
    }

    const std::vector<std::string> &PassPushConstantData::getArrItemNames() const
    {
        return m_arrItemNames;
    }
        
} //vg