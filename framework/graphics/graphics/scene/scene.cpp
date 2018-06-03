#include "graphics/scene/scene.hpp"

namespace vg
{
    BaseScene::BaseScene()
        : Base(BaseType::SCENE)
        , m_pRootTransform()
        , m_isRightHand(VG_FALSE)
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

    void BaseScene::beginRender() const
    {
        _beginRender();
    }
    void BaseScene::endRender() const
    {
        _endRender();
    }

    uint32_t BaseScene::getVisualObjectCount() const
    {
        return static_cast<uint32_t>(m_arrPVisualObjects.size());
    }
    
    uint32_t BaseScene::getCameraCount() const
    {
        return static_cast<uint32_t>(m_arrPCameras.size());
    }
    
    uint32_t BaseScene::getLightCount() const
    {
        return static_cast<uint32_t>(m_arrPLights.size());
    }


    const BaseVisualObject *BaseScene::_getVisualObjectWithIndex(uint32_t index) const
    {
        return m_arrPVisualObjects[index];
    }
    
    BaseVisualObject *BaseScene::_getVisualObjectWithIndex(uint32_t index)
    {
        return m_arrPVisualObjects[index];
    }
    
    const BaseVisualObject *BaseScene::_getVisualObjectWithTransform(const BaseTransform *pTransform) const
    {
        auto iterator = m_mapTransformIdToVisualObjects.find(pTransform->getID());
        if (iterator == m_mapTransformIdToVisualObjects.cend()) return nullptr;
        return iterator->second;
    }

    BaseVisualObject *BaseScene::_getVisualObjectWithTransform(const BaseTransform *pTransform)
    {
        auto iterator = m_mapTransformIdToVisualObjects.find(pTransform->getID());
        if (iterator == m_mapTransformIdToVisualObjects.cend()) return nullptr;
        return iterator->second;
    }
        
    Bool32 BaseScene::_isHasVisualObject(const BaseVisualObject *pTarget) const
    {
        return _isHasObject<BaseVisualObject>(pTarget, m_mapPVisualObjects);
    }
        
    void BaseScene::_addVisualObject(BaseVisualObject *pTarget, BaseVisualObject *pParent)
    {
        _addObject<BaseVisualObject>(pTarget
            , m_arrPVisualObjects
            , m_mapPVisualObjects
            , m_mapTransformIdToVisualObjects
            , m_pRootTransform.get()
            , pParent
        );
    }
        
    void BaseScene::_removeVisualObject(BaseVisualObject *pTarget)
    {
        _removeObject<BaseVisualObject>(pTarget
            , m_arrPVisualObjects
            , m_mapPVisualObjects
            , m_mapTransformIdToVisualObjects
        );
    }

    const BaseCamera *BaseScene::_getCameraWithIndex(uint32_t index) const
    {
        return m_arrPCameras[index];
    }

    BaseCamera *BaseScene::_getCameraWithIndex(uint32_t index)
    {
        return m_arrPCameras[index];
    }

    const BaseCamera *BaseScene::_getCameraWithTransform(const BaseTransform *pTransform) const
    {
        auto iterator = m_mapTransformIdToCameras.find(pTransform->getID());
        if (iterator == m_mapTransformIdToCameras.cend())throw std::runtime_error("Camera don't exist!");
        return iterator->second;
    }
        
    BaseCamera *BaseScene::_getCameraWithTransform(const BaseTransform *pTransform)
    {
        auto iterator = m_mapTransformIdToCameras.find(pTransform->getID());
        if (iterator == m_mapTransformIdToCameras.cend())throw std::runtime_error("Camera don't exist!");
        return iterator->second;
    }
    
    Bool32 BaseScene::_isHasCamera(const BaseCamera *pTarget) const
    {
        return _isHasObject<BaseCamera>(pTarget, m_mapPCameras);
    }
    
    void BaseScene::_addCamera(BaseCamera *pTarget, BaseCamera *pParent)
    {
        _addObject<BaseCamera>(pTarget
            , m_arrPCameras
            , m_mapPCameras
            , m_mapTransformIdToCameras
            , m_pRootTransform.get()
            , pParent
        );
    }
        
    void BaseScene::_removeCamera(BaseCamera *pTarget)
    {
        _removeObject<BaseCamera>(pTarget
            , m_arrPCameras
            , m_mapPCameras
            , m_mapTransformIdToCameras
        );
    }

    const BaseLight *BaseScene::_getLightWithIndex(uint32_t index) const
    {
        return m_arrPLights[index];
    }
        
    BaseLight *BaseScene::_getLightWithIndex(uint32_t index)
    {
        return m_arrPLights[index];
    }
        
    const BaseLight *BaseScene::_getLightWithTransform(const BaseTransform *pTransform) const
    {
        auto iterator = m_mapTransformIdToLights.find(pTransform->getID());
        if (iterator == m_mapTransformIdToLights.cend())throw std::runtime_error("Light don't exist!");
        return iterator->second;
    }

    BaseLight *BaseScene::_getLightWithTransform(const BaseTransform *pTransform)
    {
        auto iterator = m_mapTransformIdToLights.find(pTransform->getID());
        if (iterator == m_mapTransformIdToLights.cend())throw std::runtime_error("Light don't exist!");
        return iterator->second;
    }

    Bool32 BaseScene::_isHasLight(const BaseLight *pTarget) const
    {
        return _isHasObject<BaseLight>(pTarget, m_mapPLights);
    }
    
    void BaseScene::_addLight(BaseLight *pTarget, BaseLight *pParent)
    {
        _addObject<BaseLight>(pTarget
            , m_arrPLights
            , m_mapPLights
            , m_mapTransformIdToLights
            , m_pRootTransform.get()
            , pParent
        );
    }
        
    void BaseScene::_removeLight(BaseLight *pTarget)
    {
        _removeObject<BaseLight>(pTarget
            , m_arrPLights
            , m_mapPLights
            , m_mapTransformIdToLights
        );
    }

    void BaseScene::_beginRender() const
    {
        uint32_t len;
        len = static_cast<uint32_t>(m_arrPVisualObjects.size());
        for (uint32_t i = 0; i < len; ++i) {
            dynamic_cast<BaseObject *>(m_arrPVisualObjects[i])->beginRender();
        }
        len = static_cast<uint32_t>(m_arrPCameras.size());
        for (uint32_t i = 0; i < len; ++i) {
            dynamic_cast<BaseObject *>(m_arrPCameras[i])->beginRender();
        }
        len = static_cast<uint32_t>(m_arrPLights.size());
        for (uint32_t i = 0; i < len; ++i) {
            dynamic_cast<BaseObject *>(m_arrPLights[i])->beginRender();
        }
    }
    
    void BaseScene::_endRender() const
    { 
        uint32_t len;
        len = static_cast<uint32_t>(m_arrPVisualObjects.size());
        for (uint32_t i = 0; i < len; ++i) {
            dynamic_cast<BaseObject *>(m_arrPVisualObjects[i])->endRender();
        }
        len = static_cast<uint32_t>(m_arrPCameras.size());
        for (uint32_t i = 0; i < len; ++i) {
            dynamic_cast<BaseObject *>(m_arrPCameras[i])->endRender();
        }
        len = static_cast<uint32_t>(m_arrPLights.size());
        for (uint32_t i = 0; i < len; ++i) {
            dynamic_cast<BaseObject *>(m_arrPLights[i])->endRender();
        }
    }

    void BaseScene::_addObjectSetObjectOnly(DimObject *pTarget
            , BaseTransform *root
            , DimObject *pParent
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

    void BaseScene::_removeObjectSetObjectOnly(DimObject *pTarget)
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
        //auto children = pTransform->getChildren();
        //insert to pos of parent before target
        for (uint32_t i = 0; i < childCount; ++i) {
            auto child = pTransform->getChildWithIndex(i);
            pParent->addChild(child, pos);
        }

        //then remove all chilren of target.
        pTransform->detachChildren();
        

        pTransform->setParent(nullptr);
    }
//Scene
    template <SpaceType SPACE_TYPE>
    Scene<SPACE_TYPE>::Scene()
        : BaseScene()
    {
        m_spaceType = SPACE_TYPE;
        m_pRootTransform = std::shared_ptr<BaseTransform>(new TransformType());
    }

    template <SpaceType SPACE_TYPE>
    const typename Scene<SPACE_TYPE>::VisualObjectType *Scene<SPACE_TYPE>::getVisualObjectWithIndex(uint32_t index) const
    {
        return dynamic_cast<const VisualObjectType *>(_getVisualObjectWithIndex(index));
    }

    template <SpaceType SPACE_TYPE>
    typename Scene<SPACE_TYPE>::VisualObjectType *Scene<SPACE_TYPE>::getVisualObjectWithIndex(uint32_t index)
    {
        return dynamic_cast<VisualObjectType *>(_getVisualObjectWithIndex(index));
    }

    template <SpaceType SPACE_TYPE>
    const typename Scene<SPACE_TYPE>::VisualObjectType *Scene<SPACE_TYPE>::getVisualObjectWithTransform(const TransformType *pTransform) const
    {
        return dynamic_cast<const VisualObjectType *>(
            _getVisualObjectWithTransform(pTransform)
            );
    }

    template <SpaceType SPACE_TYPE>
    typename Scene<SPACE_TYPE>::VisualObjectType *Scene<SPACE_TYPE>::getVisualObjectWithTransform(const TransformType *pTransform)
    {
        return dynamic_cast<VisualObjectType *>(
            _getVisualObjectWithTransform(pTransform)
            );
    }

    template <SpaceType SPACE_TYPE>
    Bool32 Scene<SPACE_TYPE>::isHasVisualObject(const VisualObjectType *pTarget) const
    {
        return _isHasVisualObject(pTarget);
    }

    template <SpaceType SPACE_TYPE>
    void Scene<SPACE_TYPE>::addVisualObject(VisualObjectType *pTarget, VisualObjectType *pParent)
    {
        _addVisualObject(pTarget, pParent);
    }

    template <SpaceType SPACE_TYPE>
    void Scene<SPACE_TYPE>::removeVisualObject(VisualObjectType *pTarget)
    {
        _removeVisualObject(pTarget);
    }

    template <SpaceType SPACE_TYPE>
    const typename Scene<SPACE_TYPE>::CameraType *Scene<SPACE_TYPE>::getCameraWithIndex(uint32_t index) const
    {
        return dynamic_cast<const CameraType *>(_getCameraWithIndex(index));
    }

    template <SpaceType SPACE_TYPE>
    typename Scene<SPACE_TYPE>::CameraType *Scene<SPACE_TYPE>::getCameraWithIndex(uint32_t index)
    {
        return dynamic_cast<CameraType *>(_getCameraWithIndex(index));
    }

    template <SpaceType SPACE_TYPE>
    const typename Scene<SPACE_TYPE>::CameraType *Scene<SPACE_TYPE>::getCameraWithTransform(const TransformType *pTransform) const
    {
        return dynamic_cast<const CameraType *>(
            _getCameraWithTransform(pTransform)
            );
    }

    template <SpaceType SPACE_TYPE>
    typename Scene<SPACE_TYPE>::CameraType *Scene<SPACE_TYPE>::getCameraWithTransform(const TransformType *pTransform)
    {
        return dynamic_cast<CameraType *>(
            _getCameraWithTransform(pTransform)
            );
    }

    template <SpaceType SPACE_TYPE>
    Bool32 Scene<SPACE_TYPE>::isHasCamera(const CameraType *pTarget) const
    {
        return _isHasCamera(pTarget);
    }

    template <SpaceType SPACE_TYPE>
    void Scene<SPACE_TYPE>::addCamera(CameraType *pTarget, CameraType *pParent)
    {
        _addCamera(pTarget, pParent);
    }

    template <SpaceType SPACE_TYPE>
    void Scene<SPACE_TYPE>::removeCamera(CameraType *pTarget)
    {
        _removeCamera(pTarget);
    }

    template <SpaceType SPACE_TYPE>
    const typename Scene<SPACE_TYPE>::LightType *Scene<SPACE_TYPE>::getLightWithIndex(uint32_t index) const
    {
        return dynamic_cast<const LightType *>(_getLightWithIndex(index));
    }

    template <SpaceType SPACE_TYPE>
    typename Scene<SPACE_TYPE>::LightType *Scene<SPACE_TYPE>::getLightWithIndex(uint32_t index)
    {
        return dynamic_cast<LightType *>(_getLightWithIndex(index));
    }

    template <SpaceType SPACE_TYPE>
    const typename Scene<SPACE_TYPE>::LightType *Scene<SPACE_TYPE>::getLightWithTransform(const TransformType *pTransform) const
    {
        return dynamic_cast<const LightType *>(_getLightWithTransform(pTransform));
    }

    template <SpaceType SPACE_TYPE>
    typename Scene<SPACE_TYPE>::LightType *Scene<SPACE_TYPE>::getLightWithTransform(const TransformType *pTransform)
    {
        return dynamic_cast<LightType *>(_getLightWithTransform(pTransform));
    }

    template <SpaceType SPACE_TYPE>
    Bool32 Scene<SPACE_TYPE>::isHasLight(const LightType *pTarget) const
    {
        return _isHasLight(pTarget);
    }

    template <SpaceType SPACE_TYPE>
    void Scene<SPACE_TYPE>::addLight(LightType *pTarget, LightType *pParent)
    {
        _addLight(pTarget, pParent);
    }

    template <SpaceType SPACE_TYPE>
    void Scene<SPACE_TYPE>::removeLight(LightType *pTarget)
    {
        _removeLight(pTarget);
    }

    //template instantiation
    template class Scene<SpaceType::SPACE_2>;
    template class Scene<SpaceType::SPACE_3>;

} //namespace vg