#include "graphics/scene/scene.hpp"

namespace vg
{
    LightInfo::LightInfo(uint32_t bindingPriority
        , uint32_t maxLightCount
        , uint32_t lightDataSize
        )
        : bindingPriority(bindingPriority)
        , maxLightCount(maxLightCount)
        , lightDataSize(lightDataSize)
    {

    }

    BaseScene::BaseScene()
        : Base(BaseType::SCENE)
        , m_isRightHand(VG_FALSE)
        , m_lightDataBuffer(vk::BufferUsageFlagBits::eUniformBuffer
            , vk::MemoryPropertyFlagBits::eHostVisible)
        , m_arrRegisteredLights()
        , m_mapRegisteredLights()
    {

    }

    SpaceType BaseScene::getSpaceType() const
    {
        return m_spaceType;
    }

    Bool32 BaseScene::getIsRightHand() const
    {
        return m_isRightHand;
    }

    Bool32 BaseScene::getIsLeftHand() const
    {
       if (m_isRightHand)
       {
           return VG_FALSE;
       }
       else
       {
           return VG_TRUE;
       }
    }

    void BaseScene::setIsRightHand(Bool32 isRightHand)
    {
        m_isRightHand = isRightHand;
    }

    void BaseScene::setIsLeftHand(Bool32 isLeftHand)
    {
        if (isLeftHand)
        {
            m_isRightHand = VG_FALSE;
        }
        else
        {
            m_isRightHand = VG_TRUE;
        }
    }

    uint32_t BaseScene::getRegisterLightCount() const
    {
        return static_cast<uint32_t>(m_arrRegisteredLights.size());
    }

    Bool32 BaseScene::isHasRegisterLight(const std::type_info &lightTypeInfo) const
    {
        return m_mapRegisteredLights.count(lightTypeInfo) != 0;
    }

    void BaseScene::registerLight(const std::type_info &lightTypeInfo, const LightInfo &lightInfo)
    {
        _registerLight(lightTypeInfo, lightInfo);
    }
    
    void BaseScene::unregisterLight(const std::type_info &lightTypeInfo)
    {
        _unregisterLight(lightTypeInfo);
    }

    const BufferData &BaseScene::getLightDataBuffer() const
    {
        return m_lightDataBuffer;
    }

    void BaseScene::beginRender()
    {
        _beginRender();
    }

    void BaseScene::endRender()
    {
        _endRender();
    }

    void BaseScene::_registerLight(const std::type_info &lightTypeInfo, const LightInfo &lightInfo)
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
        _syncLightData();
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
        m_spaceType = SPACE_TYPE;
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
        if (isHasRegisterLight(typeid(*pTarget)))
            throw std::invalid_argument("The light type is not registered.");
#endif //DEBUG
        return _isHasObject<LightType>(pTarget, m_mapPLights);
    }

    template <SpaceType SPACE_TYPE>
    void Scene<SPACE_TYPE>::addLight(LightType *pTarget, LightType *pParent)
    {
        const auto &typeInfo = typeid(*pTarget);
#ifdef DEBUG
        if (isHasRegisterLight(typeInfo))
            throw std::invalid_argument("The light type is not registered.");
#endif //DEBUG
        const auto &lightInfo = m_mapRegisteredLights[std::type_index(typeInfo)];
#ifdef DEBUG
        if (getLightCount() == lightInfo.maxLightCount)
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
        if (isHasRegisterLight(typeInfo))
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
    void Scene<SPACE_TYPE>::_registerLight(const std::type_info &lightTypeInfo, const LightInfo &lightInfo)
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
    void Scene<SPACE_TYPE>::_syncLightData()
    {
        //coyy and sort array.
        auto arrRegisteredLights = m_arrRegisteredLights;
        auto &mapReigsteredLighst = m_mapRegisteredLights;
        std::sort(arrRegisteredLights.begin(), arrRegisteredLights.end(), [&](const std::type_info *item1, const std::type_info *item2) {
            const auto &lightInfo1 = mapReigsteredLighst[std::type_index(*item1)];
            const auto &lightInfo2 = mapReigsteredLighst[std::type_index(*item2)];
            return lightInfo1.bindingPriority - lightInfo2.bindingPriority;
        });

        //Caculate totol size of light data.        
        uint32_t totalSize = 0u;
        for (const auto pLightTypeInfo : arrRegisteredLights)
        {
            const auto &lightInfo = m_mapRegisteredLights[std::type_index(*pLightTypeInfo)];
            const auto &lightGroup = m_mapLightGroups[std::type_index(*pLightTypeInfo)];
            uint32_t lightCount = static_cast<uint32_t>(lightGroup.size());
            totalSize += static_cast<uint32_t>(sizeof(uint32_t));
            totalSize += lightInfo.lightDataSize * lightCount;
        }
        
        //Allocate memory and copty ligth data to it.
        std::vector<Byte> memory(totalSize);
        uint32_t offset = 0u;
        for (const auto pLightTypeInfo : arrRegisteredLights)
        {
            const auto &lightInfo = m_mapRegisteredLights[std::type_index(*pLightTypeInfo)];
            const auto &lightGroup = m_mapLightGroups[std::type_index(*pLightTypeInfo)];
            uint32_t lightCount = static_cast<uint32_t>(lightGroup.size());
            memcpy(memory.data() + offset, &lightCount, sizeof(uint32_t));
            offset += static_cast<uint32_t>(sizeof(uint32_t));
            for (const auto &pLight : lightGroup) {
                pLight->memcpyLightData(memory.data() + offset);
                offset += lightInfo.lightDataSize;
            }
        }
        m_lightDataBuffer.updateBuffer(memory.data(), totalSize);
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