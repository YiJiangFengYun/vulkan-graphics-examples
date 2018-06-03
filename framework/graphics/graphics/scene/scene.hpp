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
        void beginRender() const;
        void endRender() const;

        uint32_t getVisualObjectCount() const;
        uint32_t getCameraCount() const;
        uint32_t getLightCount() const;
        
        
    protected:
        SpaceType m_spaceType;
        Bool32 m_isRightHand;

        std::shared_ptr<BaseTransform> m_pRootTransform;

        std::vector<BaseVisualObject *> m_arrPVisualObjects;
        std::unordered_map<InstanceID, BaseVisualObject *> m_mapPVisualObjects;
        std::unordered_map<InstanceID, BaseVisualObject *> m_mapTransformIdToVisualObjects;
        std::vector<BaseCamera *> m_arrPCameras;
        std::unordered_map<InstanceID, BaseCamera *> m_mapPCameras;
        std::unordered_map<InstanceID, BaseCamera *> m_mapTransformIdToCameras;
        std::vector<BaseLight *> m_arrPLights;
        std::unordered_map<InstanceID, BaseLight *> m_mapPLights;
        std::unordered_map<InstanceID, BaseLight *> m_mapTransformIdToLights;

        const BaseVisualObject *_getVisualObjectWithIndex(uint32_t index) const;
        BaseVisualObject *_getVisualObjectWithIndex(uint32_t index);
        const BaseVisualObject *_getVisualObjectWithTransform(const BaseTransform *pTransform) const;
        BaseVisualObject *_getVisualObjectWithTransform(const BaseTransform *pTransform);
        Bool32 _isHasVisualObject(const BaseVisualObject *pTarget) const;
        void _addVisualObject(BaseVisualObject *pTarget, BaseVisualObject *pParent = nullptr);
        void _removeVisualObject(BaseVisualObject *pTarget);

        const BaseCamera *_getCameraWithIndex(uint32_t index) const;
        BaseCamera *_getCameraWithIndex(uint32_t index);
        const BaseCamera *_getCameraWithTransform(const BaseTransform *pTransform) const;
        BaseCamera *_getCameraWithTransform(const BaseTransform *pTransform);
        Bool32 _isHasCamera(const BaseCamera *pTarget) const;
        void _addCamera(BaseCamera *pTarget, BaseCamera *pParent = nullptr);
        void _removeCamera(BaseCamera *pTarget);

        const BaseLight *_getLightWithIndex(uint32_t index) const;
        BaseLight *_getLightWithIndex(uint32_t index);
        const BaseLight *_getLightWithTransform(const BaseTransform *pTransform) const;
        BaseLight *_getLightWithTransform(const BaseTransform *pTransform);
        Bool32 _isHasLight(const BaseLight *pTarget) const;
        void _addLight(BaseLight *pTarget, BaseLight *pParent = nullptr);
        void _removeLight(BaseLight *pTarget);

        virtual void _beginRender() const;
        virtual void _endRender() const;
    
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
            , BaseTransform *root
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
        
        void _addObjectSetObjectOnly(DimObject *pTarget
            , BaseTransform *root
            , DimObject *pParent = nullptr
            );
        
        void _removeObjectSetObjectOnly(DimObject *pTarget);
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


        Scene();

        const VisualObjectType *getVisualObjectWithIndex(uint32_t index) const;
        VisualObjectType *getVisualObjectWithIndex(uint32_t index);
        const VisualObjectType *getVisualObjectWithTransform(const TransformType *pTransform) const;
        VisualObjectType *getVisualObjectWithTransform(const TransformType *pTransform);
        Bool32 isHasVisualObject(const VisualObjectType *pTarget) const;
        void addVisualObject(VisualObjectType *pTarget, VisualObjectType *pParent = nullptr);
        void removeVisualObject(VisualObjectType *pTarget);

        const CameraType *getCameraWithIndex(uint32_t index) const;
        CameraType *getCameraWithIndex(uint32_t index);
        const CameraType *getCameraWithTransform(const TransformType *pTransform) const;
        CameraType *getCameraWithTransform(const TransformType *pTransform);
        Bool32 isHasCamera(const CameraType *pTarget) const;
        void addCamera(CameraType *pTarget, CameraType *pParent = nullptr);
        void removeCamera(CameraType *pTarget);

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
    private:
        
    };
} //namespace kgs

#include "graphics/scene/scene.inl"

#endif // !VG_SCENE_H
