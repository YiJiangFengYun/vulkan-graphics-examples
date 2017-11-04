#include "graphics/base.hpp"

namespace kgs
{
	std::unordered_map<BaseType, std::uint32_t> Base::m_idCreateor = {};
	Base::Base(BaseType baseType)
		: m_baseType(baseType)
		, m_id(++Base::m_idCreateor[baseType])
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