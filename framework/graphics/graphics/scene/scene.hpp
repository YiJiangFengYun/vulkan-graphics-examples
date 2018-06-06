#ifndef VG_SCENE_H
#define VG_SCENE_H

#include <vector>
#include <unordered_map>
#include "graphics/util/util.hpp"
#include "graphics/scene/object.hpp"
#include "graphics/scene/visual_object.hpp"
#include "graphics/scene/camera.hpp"
#include "graphics/scene/light.hpp"

namespace vg
{
    class BaseScene : public Base
    {
    public:
        BaseScene();
        SpaceType getSpaceType() const;
        Bool32 getIsRightHand() const;
        Bool32 getIsLeftHand() const;
        void setIsRightHand(Bool32 isRightHand);
        void setIsLeftHand(Bool32 isLeftHand);
        void beginRender();
        void endRender();
    protected:
        SpaceType m_spaceType;
        Bool32 m_isRightHand;

        virtual void _beginRender();
        virtual void _endRender();
    };

    template <SpaceType SPACE_TYPE>
    class Scene : public BaseScene
    {
    public:
        using ObjectType = Object<SPACE_TYPE>;
        using VisualObjectType = VisualObject<SPACE_TYPE>;
        using CameraType = Camera<SPACE_TYPE>;
        using LightType = Light<SPACE_TYPE>;
        using TransformType = Transform<SPACE_TYPE>;
        using BoundsType = fd::Bounds<typename SpaceTypeInfo<SPACE_TYPE>::PointType>;

        using MatrixType = typename SpaceTypeInfo<SPACE_TYPE>::MatrixType;
        using PointType = typename SpaceTypeInfo<SPACE_TYPE>::PointType;
        using MatrixVectorType = typename SpaceTypeInfo<SPACE_TYPE>::MatrixVectorType;

        const std::shared_ptr<TransformType> pRootTransform;


        Scene();

        uint32_t getVisualObjectCount() const;
        const VisualObjectType *getVisualObjectWithIndex(uint32_t index) const;
        VisualObjectType *getVisualObjectWithIndex(uint32_t index);
        const VisualObjectType *getVisualObjectWithTransform(const TransformType *pTransform) const;
        VisualObjectType *getVisualObjectWithTransform(const TransformType *pTransform);
        Bool32 isHasVisualObject(const VisualObjectType *pTarget) const;
        void addVisualObject(VisualObjectType *pTarget, VisualObjectType *pParent = nullptr);
        void removeVisualObject(VisualObjectType *pTarget);

        uint32_t getCameraCount() const;
        const CameraType *getCameraWithIndex(uint32_t index) const;
        CameraType *getCameraWithIndex(uint32_t index);
        const CameraType *getCameraWithTransform(const TransformType *pTransform) const;
        CameraType *getCameraWithTransform(const TransformType *pTransform);
        Bool32 isHasCamera(const CameraType *pTarget) const;
        void addCamera(CameraType *pTarget, CameraType *pParent = nullptr);
        void removeCamera(CameraType *pTarget);

        uint32_t getLightCount() const;
        const LightType *getLightWithIndex(uint32_t index) const;
        LightType *getLightWithIndex(uint32_t index);
        const LightType *getLightWithTransform(const TransformType *pTransform) const;
        LightType *getLightWithTransform(const TransformType *pTransform);
        Bool32 isHasLight(const LightType *pTarget) const;
        void addLight(LightType *pTarget, LightType *pParent = nullptr);
        void removeLight(LightType *pTarget);

        /*Get projective matrix with camera, it will change origin projective matrix of the camera
          with handed type info.*/
        virtual MatrixType getProjMatrix(const CameraType *pCamera) const = 0;
        virtual BoundsType getViewBoundsInWorld(const CameraType *pCamera) const = 0;
        /**
         * This bounds is in object coordinate system. 
         **/
        virtual Bool32 isInView(const CameraType *pCamera
            , const TransformType *pTransform
            , BoundsType bounds
            , fd::Rect2D *viewRect = nullptr) const = 0;
        
        /**
         * This bounds is in world coorindate system.
         **/
        virtual Bool32 isInView(const CameraType *pCamera
            , BoundsType bounds
            , fd::Rect2D *viewRect = nullptr) const = 0;

    protected:

        //aggregations
        std::vector<VisualObjectType *> m_arrPVisualObjects;
        std::unordered_map<InstanceID, VisualObjectType *> m_mapPVisualObjects;
        std::unordered_map<InstanceID, VisualObjectType *> m_mapTransformIdToVisualObjects;
        std::vector<CameraType *> m_arrPCameras;
        std::unordered_map<InstanceID, CameraType *> m_mapPCameras;
        std::unordered_map<InstanceID, CameraType *> m_mapTransformIdToCameras;
        std::vector<LightType *> m_arrPLights;
        std::unordered_map<InstanceID, LightType *> m_mapPLights;
        std::unordered_map<InstanceID, LightType *> m_mapTransformIdToLights;
        // virtual void _addVisualObject(VisualObjectType *pTarget
        //     , VisualObjectType *pParent);
        // virtual void _removeVisualObject(VisualObjectType *pTarget);

        // virtual void _registerLight(const std::type_info &lightTypeInfo, const LightInfo &lightInfo) override;
        // virtual void _unregisterLight(const std::type_info &lightTypeInfo) override;

        virtual void _beginRender() override;
        virtual void _endRender() override;
    private:
        template <typename T>
        Bool32 _isHasObject(const T *pTarget
            , const std::unordered_map<InstanceID, T *> &map
        ) const;

        template <typename T>
        void _addObject(T *pTarget
            , std::vector<T *> &arr
            , std::unordered_map<InstanceID, T *> &map
            , std::unordered_map<InstanceID, T *> &mapTransformToObjs
            , TransformType *root
            , T *pParent = nullptr
        );

        template <typename T>
        void _removeObject(T *pTarget
            , std::vector<T *> &arr
            , std::unordered_map<InstanceID, T *> &map
            , std::unordered_map<InstanceID, T *> &mapTransformToObjs
        );

        template <typename T>
        void _addObjectAddSceneDataOnly(T *pTarget
            , std::vector<T *> &arr
            , std::unordered_map<InstanceID, T *> &map
            , std::unordered_map<InstanceID, T *> &mapTransformToObjs
        );

        template <typename T>
        void _removeObjectDeleteSceneDataOnly(T *pTarget
            , std::vector<T *> &arr
            , std::unordered_map<InstanceID, T *> &map
            , std::unordered_map<InstanceID, T *> &mapTransformToObjs
        );
        
        void _addObjectSetObjectOnly(ObjectType *pTarget
            , TransformType *root
            , ObjectType *pParent = nullptr
            );
        
        void _removeObjectSetObjectOnly(ObjectType *pTarget);
    };
} //namespace kgs

#include "graphics/scene/scene.inl"

#endif // !VG_SCENE_H
