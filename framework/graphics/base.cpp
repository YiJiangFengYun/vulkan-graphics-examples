#include "graphics/base.hpp"

namespace vg
{
	std::unordered_map<BaseType, InstanceID> Base::m_idCreator = {};
	Base::Base(BaseType baseType)
		: m_baseType(baseType)
		, m_id(++Base::m_idCreator[baseType])
		, m_stateID(VG_BASE_INIT_STATE_ID)
	{
		
	}

	BaseType Base::getBaseType()
	{
		return m_baseType;
	}

	uint32_t Base::getID()
	{
		return m_id;
	}

	StateID Base::getStateID()
	{
		return m_stateID;
	}
	
	void Base::updateStateID()
	{
		++m_stateID;
		if ( m_stateID == std::numeric_limits<uint32_t>::max())
		{
			m_stateID = VG_BASE_INIT_STATE_ID;
		}
	}

}