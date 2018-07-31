#include "graphics/scene/scene.hpp"

namespace vg
{
    SceneLightRegisterInfo::SceneLightRegisterInfo(uint32_t bindingPriority
        , uint32_t maxCount
        , uint32_t dataSize
        , uint32_t textureCount
        )
        : bindingPriority(bindingPriority)
        , maxCount(maxCount)
        , dataSize(dataSize)
        , textureCount(textureCount)
    {

    }

    BaseScene::BaseScene()
        : Base(BaseType::SCENE)
        , m_space()
        , m_arrRegisteredLights()
        , m_mapRegisteredLights()
    {

    }

    SpaceType BaseScene::getSpaceType() const
    {
        return m_space.spaceType;
    }

    Bool32 BaseScene::getIsRightHand() const
    {
        return m_space.rightHand;
    }

    Bool32 BaseScene::getIsLeftHand() const
    {
        return ! m_space.rightHand;
    }

    void BaseScene::setIsRightHand(Bool32 isRightHand)
    {
        m_space.rightHand = isRightHand;
    }

    void BaseScene::setIsLeftHand(Bool32 isLeftHand)
    {
        m_space.rightHand = ! isLeftHand;
    }

    const Space &BaseScene::getSpace() const
    {
        return m_space;
    }

    uint32_t BaseScene::getRegisterLightCount() const
    {
        return static_cast<uint32_t>(m_arrRegisteredLights.size());
    }

    Bool32 BaseScene::isHasRegisterLight(const std::type_info &lightTypeInfo) const
    {
        return m_mapRegisteredLights.count(lightTypeInfo) != 0;
    }

    const std::vector<const std::type_info *> BaseScene::getArrRegisteredLights() const
    {
        return m_arrRegisteredLights;
    }

    const std::unordered_map<std::type_index, SceneLightRegisterInfo> BaseScene::getMapRegisteredLights() const
    {
        return m_mapRegisteredLights;
    }

    void BaseScene::registerLight(const std::type_info &lightTypeInfo, const SceneLightRegisterInfo &lightInfo)
    {
        _registerLight(lightTypeInfo, lightInfo);
    }
    
    void BaseScene::unregisterLight(const std::type_info &lightTypeInfo)
    {
        _unregisterLight(lightTypeInfo);
    }

    void BaseScene::beginRender()
    {
        _beginRender();
    }

    void BaseScene::endRender()
    {
        _endRender();
    }

    void BaseScene::_registerLight(const std::type_info &lightTypeInfo, const SceneLightRegisterInfo &lightInfo)
    {
        {
            const auto &iterator = m_mapRegisteredLights.find(std::type_index(lightTypeInfo));
            if (iterator != m_mapRegisteredLights.cend())
            {
                throw std::runtime_error("Repeatedly register the same light.");
            }
            m_mapRegisteredLights.insert({lightTypeInfo, lightInfo});
        }
        {
            auto iterator = std::find(m_arrRegisteredLights.begin(), m_arrRegisteredLights.end(), &lightTypeInfo);
            if (iterator != m_arrRegisteredLights.end())
            {
                throw std::runtime_error("Repeatedly register the same light.");
            }
            m_arrRegisteredLights.push_back(&lightTypeInfo);
        }
    }
    
    void BaseScene::_unregisterLight(const std::type_info &lightTypeInfo)
    {
        m_mapRegisteredLights.erase(std::type_index(lightTypeInfo));
        auto iterator = std::find(m_arrRegisteredLights.begin(), m_arrRegisteredLights.end(), &lightTypeInfo);
        m_arrRegisteredLights.erase(iterator);
    }

    void BaseScene::_beginRender()
    {
    }
    
    void BaseScene::_endRender()
    { 
        
    }

//Scene
    template <SpaceType SPACE_TYPE>
    Scene<SPACE_TYPE>::Scene()
        : BaseScene()
        , pRootTransform(new TransformType())
    {
        m_space.spaceType = SPACE_TYPE;
    }

    template <SpaceType SPACE_TYPE>
    uint32_t Scene<SPACE_TYPE>::getVisualObjectCount() const
    {
        return static_cast<uint32_t>(m_arrPVisualObjects.size());
    }

    template <SpaceType SPACE_TYPE>
    const typename Scene<SPACE_TYPE>::VisualObjectType *Scene<SPACE_TYPE>::getVisualObjectWithIndex(uint32_t index) const
    {
        return m_arrPVisualObjects[index];
    }

    template <SpaceType SPACE_TYPE>
    typename Scene<SPACE_TYPE>::VisualObjectType *Scene<SPACE_TYPE>::getVisualObjectWithIndex(uint32_t index)
    {
        return m_arrPVisualObjects[index];
    }

    template <SpaceType SPACE_TYPE>
    const typename Scene<SPACE_TYPE>::VisualObjectType *Scene<SPACE_TYPE>::getVisualObjectWithTransform(const TransformType *pTransform) const
    {
        auto iterator = m_mapTransformIdToVisualObjects.find(pTransform->getID());
        if (iterator == m_mapTransformIdToVisualObjects.cend()) return nullptr;
        return iterator->second;
    }

    template <SpaceType SPACE_TYPE>
    typename Scene<SPACE_TYPE>::VisualObjectType *Scene<SPACE_TYPE>::getVisualObjectWithTransform(const TransformType *pTransform)
    {
        auto iterator = m_mapTransformIdToVisualObjects.find(pTransform->getID());
        if (iterator == m_mapTransformIdToVisualObjects.cend()) return nullptr;
        return iterator->second;
    }

    template <SpaceType SPACE_TYPE>
    Bool32 Scene<SPACE_TYPE>::isHasVisualObject(const VisualObjectType *pTarget) const
    {
        return _isHasObject<VisualObjectType>(pTarget, m_mapPVisualObjects);
    }

    template <SpaceType SPACE_TYPE>
    void Scene<SPACE_TYPE>::addVisualObject(VisualObjectType *pTarget, VisualObjectType *pParent)
    {
        _addObject<VisualObjectType>(pTarget
            , m_arrPVisualObjects
            , m_mapPVisualObjects
            , m_mapTransformIdToVisualObjects
            , pRootTransform.get()
            , pParent
        );
    }

    template <SpaceType SPACE_TYPE>
    void Scene<SPACE_TYPE>::removeVisualObject(VisualObjectType *pTarget)
    {
        _removeObject<VisualObjectType>(pTarget
            , m_arrPVisualObjects
            , m_mapPVisualObjects
            , m_mapTransformIdToVisualObjects
        );
    }

    template <SpaceType SPACE_TYPE>
    uint32_t Scene<SPACE_TYPE>::getCameraCount() const
    {
        return static_cast<uint32_t>(m_arrPCameras.size());
    }

    template <SpaceType SPACE_TYPE>
    const typename Scene<SPACE_TYPE>::CameraType *Scene<SPACE_TYPE>::getCameraWithIndex(uint32_t index) const
    {
        return m_arrPCameras[index];
    }

    template <SpaceType SPACE_TYPE>
    typename Scene<SPACE_TYPE>::CameraType *Scene<SPACE_TYPE>::getCameraWithIndex(uint32_t index)
    {
        return m_arrPCameras[index];
    }

    template <SpaceType SPACE_TYPE>
    const typename Scene<SPACE_TYPE>::CameraType *Scene<SPACE_TYPE>::getCameraWithTransform(const TransformType *pTransform) const
    {
        auto iterator = m_mapTransformIdToCameras.find(pTransform->getID());
        if (iterator == m_mapTransformIdToCameras.cend())throw std::runtime_error("Camera don't exist!");
        return iterator->second;
    }

    template <SpaceType SPACE_TYPE>
    typename Scene<SPACE_TYPE>::CameraType *Scene<SPACE_TYPE>::getCameraWithTransform(const TransformType *pTransform)
    {
        auto iterator = m_mapTransformIdToCameras.find(pTransform->getID());
        if (iterator == m_mapTransformIdToCameras.cend())throw std::runtime_error("Camera don't exist!");
        return iterator->second;
    }

    template <SpaceType SPACE_TYPE>
    Bool32 Scene<SPACE_TYPE>::isHasCamera(const CameraType *pTarget) const
    {
        return _isHasObject<CameraType>(pTarget, m_mapPCameras);
    }

    template <SpaceType SPACE_TYPE>
    void Scene<SPACE_TYPE>::addCamera(CameraType *pTarget, CameraType *pParent)
    {
        _addObject<CameraType>(pTarget
            , m_arrPCameras
            , m_mapPCameras
            , m_mapTransformIdToCameras
            , pRootTransform.get()
            , pParent
        );
    }

    template <SpaceType SPACE_TYPE>
    void Scene<SPACE_TYPE>::removeCamera(CameraType *pTarget)
    {
        _removeObject<CameraType>(pTarget
            , m_arrPCameras
            , m_mapPCameras
            , m_mapTransformIdToCameras
        );
    }

    template <SpaceType SPACE_TYPE>
    uint32_t Scene<SPACE_TYPE>::getLightCount() const
    {
        return static_cast<uint32_t>(m_arrPLights.size());
    }

    template <SpaceType SPACE_TYPE>
    const typename Scene<SPACE_TYPE>::LightType *Scene<SPACE_TYPE>::getLightWithIndex(uint32_t index) const
    {
        return m_arrPLights[index];
    }

    template <SpaceType SPACE_TYPE>
    typename Scene<SPACE_TYPE>::LightType *Scene<SPACE_TYPE>::getLightWithIndex(uint32_t index)
    {
        return m_arrPLights[index];
    }

    template <SpaceType SPACE_TYPE>
    const typename Scene<SPACE_TYPE>::LightType *Scene<SPACE_TYPE>::getLightWithTransform(const TransformType *pTransform) const
    {
        auto iterator = m_mapTransformIdToLights.find(pTransform->getID());
        if (iterator == m_mapTransformIdToLights.cend())throw std::runtime_error("Light don't exist!");
        return iterator->second;
    }

    template <SpaceType SPACE_TYPE>
    typename Scene<SPACE_TYPE>::LightType *Scene<SPACE_TYPE>::getLightWithTransform(const TransformType *pTransform)
    {
        auto iterator = m_mapTransformIdToLights.find(pTransform->getID());
        if (iterator == m_mapTransformIdToLights.cend())throw std::runtime_error("Light don't exist!");
        return iterator->second;
    }

    template <SpaceType SPACE_TYPE>
    Bool32 Scene<SPACE_TYPE>::isHasLight(const LightType *pTarget) const
    {
#ifdef DEBUG
        if (isHasRegisterLight(typeid(*pTarget)) == VG_FALSE)
            throw std::invalid_argument("The light type is not registered.");
#endif //DEBUG
        return _isHasObject<LightType>(pTarget, m_mapPLights);
    }

    template <SpaceType SPACE_TYPE>
    void Scene<SPACE_TYPE>::addLight(LightType *pTarget, LightType *pParent)
    {
        const auto &typeInfo = typeid(*pTarget);
#ifdef DEBUG
        if (isHasRegisterLight(typeInfo) == VG_FALSE)
            throw std::invalid_argument("The light type is not registered.");
#endif //DEBUG
        const auto &lightInfo = m_mapRegisteredLights[std::type_index(typeInfo)];
#ifdef DEBUG
        const auto &lightGroup = getLightGroup(typeInfo);
        if (static_cast<uint32_t>(lightGroup.size()) == lightInfo.maxCount)
            throw std::runtime_error("Light count of the scene must not more than max light count of the scene.");
#endif //DEBUG
        _addObject<LightType>(pTarget
            , m_arrPLights
            , m_mapPLights
            , m_mapTransformIdToLights
            , pRootTransform.get()
            , pParent
        );

        m_mapLightGroups[std::type_index(typeInfo)].push_back(pTarget);
    }

    template <SpaceType SPACE_TYPE>
    void Scene<SPACE_TYPE>::removeLight(LightType *pTarget)
    {
        const auto &typeInfo = typeid(*pTarget);        
#ifdef DEBUG
        if (isHasRegisterLight(typeInfo) == VG_FALSE)
            throw std::invalid_argument("The light type is not registered.");
#endif //DEBUG
        _removeObject<LightType>(pTarget
            , m_arrPLights
            , m_mapPLights
            , m_mapTransformIdToLights
        );

        auto &lightGroup = m_mapLightGroups[std::type_index(typeInfo)];
        auto iterator = std::find(lightGroup.begin(), lightGroup.end(), pTarget);
        lightGroup.erase(iterator);
    }

    template <SpaceType SPACE_TYPE>
    const std::vector<typename Scene<SPACE_TYPE>::LightType *> &Scene<SPACE_TYPE>::getLightGroup(const std::type_info &lightTypeInfo)
    {
        if (isHasRegisterLight(lightTypeInfo) == VG_FALSE) {
            throw std::invalid_argument("The light type don't be registered!");
        }
        else {
            return m_mapLightGroups[std::type_index(lightTypeInfo)];
        }
    }

    template <SpaceType SPACE_TYPE>
    void Scene<SPACE_TYPE>::_registerLight(const std::type_info &lightTypeInfo, const SceneLightRegisterInfo &lightInfo)
    {
        BaseScene::_registerLight(lightTypeInfo, lightInfo);
    }
    
    template <SpaceType SPACE_TYPE>
    void Scene<SPACE_TYPE>::_unregisterLight(const std::type_info &lightTypeInfo)
    {
        BaseScene::_unregisterLight(lightTypeInfo);
        m_mapLightGroups.erase(lightTypeInfo);
    }

    template <SpaceType SPACE_TYPE>
    void Scene<SPACE_TYPE>::_beginRender()
    {
        BaseScene::_beginRender();
        uint32_t len;
        len = static_cast<uint32_t>(m_arrPVisualObjects.size());
        for (uint32_t i = 0; i < len; ++i) {
            m_arrPVisualObjects[i]->beginRender();
        }
        len = static_cast<uint32_t>(m_arrPCameras.size());
        for (uint32_t i = 0; i < len; ++i) {
            m_arrPCameras[i]->beginRender();
        }
        len = static_cast<uint32_t>(m_arrPLights.size());
        for (uint32_t i = 0; i < len; ++i) {
            m_arrPLights[i]->beginRender();
        }
    }
    
    template <SpaceType SPACE_TYPE>
    void Scene<SPACE_TYPE>::_endRender()
    {
        uint32_t len;
        len = static_cast<uint32_t>(m_arrPVisualObjects.size());
        for (uint32_t i = 0; i < len; ++i) {
            m_arrPVisualObjects[i]->endRender();
        }
        len = static_cast<uint32_t>(m_arrPCameras.size());
        for (uint32_t i = 0; i < len; ++i) {
            m_arrPCameras[i]->endRender();
        }
        len = static_cast<uint32_t>(m_arrPLights.size());
        for (uint32_t i = 0; i < len; ++i) {
            m_arrPLights[i]->endRender();
        }
        BaseScene::_endRender();
    }

    template <SpaceType SPACE_TYPE>
    void Scene<SPACE_TYPE>::_addObjectSetObjectOnly(ObjectType *pTarget
            , TransformType *root
            , ObjectType *pParent
            )
    {
#ifdef DEBUG
        //chilren of target
        if (pTarget->getTransform()->getChildCount())
            throw std::invalid_argument("Target should not own chilren.");
#endif // DEBUG
        pTarget->addToSpace(m_space);
        //construct hierarchy data.
        if (pParent != nullptr)
        {
            pTarget->getTransform()->setParent(dynamic_cast<TransformType *>(pParent->getTransform()));
        }
        else
        {
            pTarget->getTransform()->setParent(root);
        }
    }
    
    template <SpaceType SPACE_TYPE>
    void Scene<SPACE_TYPE>::_removeObjectSetObjectOnly(ObjectType *pTarget)
    {
        ////clear hierarchy data.
        //pTarget->getTransform()->setParent(nullptr);
        //pTarget->getTransform()->detachChildren();

        //reconstruct hierarchy.
        //connect between children and parent of target.
        auto pTransform = pTarget->getTransform();
        auto pParent = pTransform->getParent();
        auto pos = pParent->getChildPos(pTransform);
        //copy chilren refs;
        auto childCount = pTransform->getChildCount();
        auto children = pTransform->getChildren();
        //insert to pos of parent before target
        for (uint32_t i = 0; i < childCount; ++i) {
            auto child = *(children + i);
            pParent->addChild(child, pos);
        }

        //then remove all chilren of target.
        pTransform->detachChildren();
        

        pTransform->setParent(nullptr);
    }

    //template instantiation
    template class Scene<SpaceType::SPACE_2>;
    template class Scene<SpaceType::SPACE_3>;

} //namespace vg