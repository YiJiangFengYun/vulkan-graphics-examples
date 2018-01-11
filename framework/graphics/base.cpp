#include "graphics/base.hpp"

namespace kgs
{
	std::unordered_map<BaseType, InstanceID> Base::m_idCreator = {};
	Base::Base(BaseType baseType)
		: m_baseType(baseType)
		, m_id(++Base::m_idCreator[baseType])
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

}