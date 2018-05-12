#include "graphics/scene/scene.hpp"

namespace vg
{
	BaseScene::BaseScene()
		: Base(BaseType::SCENE)
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

//Scene
    template <SpaceType SPACE_TYPE>
	Scene<SPACE_TYPE>::Scene()
		: BaseScene()
		, pRootTransformForVisualObject(new TransformType())
		, pRootTransformForCamera(new TransformType())
		, pRootTransformForLight(new TransformType())
	{
		m_spaceType = SPACE_TYPE;
	}

	template <SpaceType SPACE_TYPE>
	uint32_t Scene<SPACE_TYPE>::getVisualObjectCount() const
	{
		return static_cast<uint32_t>(m_arrPVisualObjects.size());
	}

	template <SpaceType SPACE_TYPE>
	typename Scene<SPACE_TYPE>::VisualObjectType *Scene<SPACE_TYPE>::getVisualObjectWithIndex(uint32_t index) const
	{
		return m_arrPVisualObjects[index];
	}

	template <SpaceType SPACE_TYPE>
	typename Scene<SPACE_TYPE>::VisualObjectType *Scene<SPACE_TYPE>::getVisualObjectWithTransform(const TransformType *pTransform) const
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
		_addVisualObject(pTarget, pParent);
	}

	template <SpaceType SPACE_TYPE>
	void Scene<SPACE_TYPE>::removeVisualObject(VisualObjectType *pTarget)
	{
		_removeVisualObject(pTarget);
	}

	template <SpaceType SPACE_TYPE>
	uint32_t Scene<SPACE_TYPE>::getCameraCount() const
	{
		return static_cast<uint32_t>(m_arrPCameras.size());
	}

	template <SpaceType SPACE_TYPE>
	typename Scene<SPACE_TYPE>::CameraType *Scene<SPACE_TYPE>::getCameraWithIndex(uint32_t index) const
	{
		return m_arrPCameras[index];
	}

	template <SpaceType SPACE_TYPE>
	typename Scene<SPACE_TYPE>::CameraType *Scene<SPACE_TYPE>::getCameraWithTransform(const TransformType *pTransform) const
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
			, pRootTransformForCamera.get()
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
	typename Scene<SPACE_TYPE>::LightType *Scene<SPACE_TYPE>::getLightWithIndex(uint32_t index) const
	{
		return m_arrPLights[index];
	}

	template <SpaceType SPACE_TYPE>
	typename Scene<SPACE_TYPE>::LightType *Scene<SPACE_TYPE>::getLightWithTransform(const TransformType *pTransform) const
	{
		auto iterator = m_mapTransformIdToLights.find(pTransform->getID());
		if (iterator == m_mapTransformIdToLights.cend())throw std::runtime_error("Light don't exist!");
		return iterator->second;
	}

	template <SpaceType SPACE_TYPE>
	Bool32 Scene<SPACE_TYPE>::isHasLight(const LightType *pTarget) const
	{
		return _isHasObject<LightType>(pTarget, m_mapPLights);
	}

	template <SpaceType SPACE_TYPE>
	void Scene<SPACE_TYPE>::addLight(LightType *pTarget, LightType *pParent)
	{
		_addObject<LightType>(pTarget
			, m_arrPLights
			, m_mapPLights
			, m_mapTransformIdToLights
			, pRootTransformForLight.get()
			, pParent
		);
	}

	template <SpaceType SPACE_TYPE>
	void Scene<SPACE_TYPE>::removeLight(LightType *pTarget)
	{
		_removeObject<LightType>(pTarget
			, m_arrPLights
			, m_mapPLights
			, m_mapTransformIdToLights
		);
	}

	template <SpaceType SPACE_TYPE>
	void Scene<SPACE_TYPE>::_addVisualObject(VisualObjectType *pTarget
		, VisualObjectType *pParent)
	{
		_addObject<VisualObjectType>(pTarget
			, m_arrPVisualObjects
			, m_mapPVisualObjects
			, m_mapTransformIdToVisualObjects
			, pRootTransformForVisualObject.get()
			, pParent
		);
	}

	template <SpaceType SPACE_TYPE>
	void Scene<SPACE_TYPE>::_removeVisualObject(VisualObjectType *pTarget)
	{
		_removeObject<VisualObjectType>(pTarget
			, m_arrPVisualObjects
			, m_mapPVisualObjects
			, m_mapTransformIdToVisualObjects
		);
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
		auto children = pTransform->getChildren();
		//first remove all chilren of target.
		pTransform->detachChildren();
		//insert to pos of parent before target
		for (auto child : children)
		{
			pParent->addChild(child, pos);
		}

		pTransform->setParent(nullptr);
	}

	//template instantiation
    template class Scene<SpaceType::SPACE_2>;
	template class Scene<SpaceType::SPACE_3>;

} //namespace vg