#include "graphics/scene/object.hpp"

namespace vg
{
//BaseObject
    BaseObject::BaseObject()
        : m_objectType(ObjectType::UNDEFINED)
        , m_space()
    {

    }

    ObjectType BaseObject::getObjectType()
    {
        return m_objectType;
    }

    void BaseObject::addToSpace(const Space &space)
    {
        _addToSpace(space);
    }

    const Space &BaseObject::getSpace() const
    {
        return m_space;
    }

    void BaseObject::beginRender()
    {
        _beginRender();
    }

    void BaseObject::endRender()
    {
        _endRender();
    }

    void BaseObject::_beginRender()
    {

    }
    
    void BaseObject::_endRender()
    {

    }

    void BaseObject::_addToSpace(const Space &space)
    {
        m_space = space;
    }


//DimObject
    template <SpaceType SPACE_TYPE>
    DimObject<SPACE_TYPE>::DimObject()
        : m_pTransform(new TransformType())
    {
    }
  
    template <SpaceType SPACE_TYPE>
    typename DimObject<SPACE_TYPE>::TransformType * DimObject<SPACE_TYPE>::getTransform() const
    {
        return m_pTransform.get();
    }

    //template instantiation
    template class DimObject<SpaceType::SPACE_2>;
    template class DimObject<SpaceType::SPACE_3>;


//Object
    template <SpaceType SPACE_TYPE>
    Object<SPACE_TYPE>::Object()
        : BaseObject()
        , DimObject<SPACE_TYPE>()
    {
        m_pTransform->setSpace(m_space);
    }

    template <SpaceType SPACE_TYPE>
    void Object<SPACE_TYPE>::_addToSpace(const Space &space)
    {
        m_pTransform->setSpace(space);
    }


    //template instantiation
    template class Object<SpaceType::SPACE_2>;
    template class Object<SpaceType::SPACE_3>;

} //namespace kgs