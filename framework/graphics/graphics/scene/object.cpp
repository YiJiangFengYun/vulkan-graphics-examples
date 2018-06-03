#include "graphics/scene/object.hpp"

namespace vg
{
//BaseObject
    BaseObject::BaseObject()
        : m_pTransform()
    {

    }

    ObjectType BaseObject::getObjectType()
    {
        return m_objectType;
    }

    void BaseObject::beginRender() const
    {
        _beginRender();
    }

    void BaseObject::endRender() const
    {
        _endRender();
    }

    void BaseObject::_beginRender() const
    {

    }
    
    void BaseObject::_endRender() const
    {

    }


//DimObject
    template <SpaceType SPACE_TYPE>
    DimObject<SPACE_TYPE>::DimObject()
        : BaseObject()
    {
        m_pTransform = std::shared_ptr<BaseTransform>(new TransformType());
    }

    template <SpaceType SPACE_TYPE>
    const typename DimObject<SPACE_TYPE>::TransformType *DimObject<SPACE_TYPE>::getTransform() const
    {
        return dynamic_cast<const TransformType *>(m_pTransform.get());
    }

    template <SpaceType SPACE_TYPE>
    typename DimObject<SPACE_TYPE>::TransformType *DimObject<SPACE_TYPE>::getTransform()
    {
        return dynamic_cast<TransformType *>(m_pTransform.get());
    }

    //template instantiation
    template class DimObject<SpaceType::SPACE_2>;
    template class DimObject<SpaceType::SPACE_3>;


//Object
    template <SpaceType SPACE_TYPE>
    Object<SPACE_TYPE>::Object()
        : DimObject<SPACE_TYPE>()
    {

    }

    //template instantiation
    template class Object<SpaceType::SPACE_2>;
    template class Object<SpaceType::SPACE_3>;

} //namespace kgs