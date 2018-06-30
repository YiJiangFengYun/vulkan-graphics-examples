#ifndef VG_SCENE_H
#define VG_SCENE_H

#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include "graphics/util/util.hpp"
#include "graphics/scene/object.hpp"
#include "graphics/scene/visual_object.hpp"
#include "graphics/scene/camera.hpp"
#include "graphics/scene/light.hpp"
#include "graphics/buffer_data/buffer_data.hpp"

#define VG_DEFAULT_SCENE_MAX_LIGHT_COUNT 10u
namespace vg
{
    struct SceneLightRegisterInfo {
        uint32_t bindingPriority;
        uint32_t maxCount;
        uint32_t dataSize;
        uint32_t textureCount;
        SceneLightRegisterInfo(uint32_t bindingPriority = 0u
            , uint32_t maxCount = VG_DEFAULT_SCENE_MAX_LIGHT_COUNT
            , uint32_t dataSize = 0u
            , uint32_t textureCount = 0u
            );
    };
    class BaseScene : public Base
    {
    public:
        BaseScene();
        SpaceType getSpaceType() const;
        Bool32 getIsRightHand() const;
        Bool32 getIsLeftHand() const;
        void setIsRightHand(Bool32 isRightHand);
        void setIsLeftHand(Bool32 isLeftHand);
        const Space &getSpace() const;
        uint32_t getRegisterLightCount() const;
        Bool32 isHasRegisterLight(const std::type_info &lightTypeInfo) const;
        const std::vector<const std::type_info *> getArrRegisteredLights() const;
        const std::unordered_map<std::type_index, SceneLightRegisterInfo> getMapRegisteredLights() const;
        void registerLight(const std::type_info &lightTypeInfo, const SceneLightRegisterInfo &lightInfo);
        void unregisterLight(const std::type_info &lightTypeInfo);
        void beginRender();
        void endRender();
    protected:
        Space m_space;
        std::vector<const std::type_info *> m_arrRegisteredLights;
        std::unordered_map<std::type_index, SceneLightRegisterInfo> m_mapRegisteredLights;

        virtual void _registerLight(const std::type_info &lightTypeInfo, const SceneLightRegisterInfo &lightInfo);
        virtual void _unregisterLight(const std::type_info &lightTypeInfo);
        virtual void _beginRender();
        virtual void _endRender();
    };

    template <SpaceType SPACE_TYPE>
    class Scene : public BaseScene
    {
    public:
        using ObjectType = Object<SPACE_TYPE>;
        using ProjectorType = Projector<SPACE_TYPE>;
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
        const std::vector<LightType *> getLightGroup(const std::type_info &lightTypeInfo);

        /*Get projective matrix with camera, it will change origin projective matrix of the camera
          with handed type info.*/
        virtual MatrixType getProjMatrix(const ProjectorType *pProjector) const = 0;
        virtual BoundsType getProjectionBoundsInWorld(const ProjectorType *pProjector) const = 0;
        /**
         * This bounds is in object coordinate system. 
         **/
        virtual Bool32 isInProjection(const ProjectorType *pProjector
            , const TransformType *pTransform
            , BoundsType bounds
            , fd::Rect2D *projectionRect = nullptr) const = 0;
        
        /**
         * This bounds is in world coorindate system.
         **/
        virtual Bool32 isInProjection(const ProjectorType *pProjector
            , BoundsType bounds
            , fd::Rect2D *projectionRect = nullptr) const = 0;

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
        std::unordered_map<std::type_index, std::vector<LightType *>> m_mapLightGroups;

        virtual void _registerLight(const std::type_info &lightTypeInfo, const SceneLightRegisterInfo &lightInfo) override;
        virtual void _unregisterLight(const std::type_info &lightTypeInfo) override;

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
