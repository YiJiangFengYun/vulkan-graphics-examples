#ifndef VG_OBJECT_H
#define VG_OBJECT_H

#include "graphics/global.hpp"
#include "graphics/scene/space_info.hpp"
#include "graphics/scene/space_info_object.hpp"

namespace vg
{
    enum class ObjectType
    {
        UNDEFINED,
        VISUAL_OBJECT,
        CAMERA,
        LIGHT,
        BEGIN_RANGE = VISUAL_OBJECT,
        END_RANGE = LIGHT,
        RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
    };

    //This class is used for object base features.
    class BaseObject
    {
    public:
        BaseObject();
        ObjectType getObjectType();
        //This method is called in scene when it is added to this scene.
        void addToSpace(const Space &space);
        const Space &getSpace() const;
        void beginRender();
        void endRender();
    protected:
        ObjectType m_objectType;
        Space m_space;
        virtual void _beginRender();
        virtual void _endRender();
        virtual void _addToSpace(const Space &space);
    };

    //This class is used for object dimension space features.
    template <SpaceType SPACE_TYPE>
    class DimObject
    {
    public:
        using TransformType = typename SpaceTransformTypeInfo<SPACE_TYPE>::TransformType;
        DimObject();

        TransformType *getTransform() const;
    protected:
        std::shared_ptr<TransformType> m_pTransform;
    };

    template <SpaceType SPACE_TYPE>
    class Object : public BaseObject, public DimObject<SPACE_TYPE>
    {
    public:
        Object();
        virtual void _addToSpace(const Space &space) override;
    protected:
        
    };

} //namespace kgs

#endif // !VG_OBJECT_H
