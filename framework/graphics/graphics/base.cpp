#include "graphics/base.hpp"

namespace vg
{
    std::unordered_map<BaseType, InstanceID> Base::m_idCreator = {};
    Base::Base(BaseType baseType)
        : m_baseType(baseType)
        , m_id(++Base::m_idCreator[baseType])
    {
        
    }

    Base::~Base()
    {
        
    }

    BaseType Base::getBaseType() const
    {
        return m_baseType;
    }

    uint32_t Base::getID() const
    {
        return m_id;
    }
}