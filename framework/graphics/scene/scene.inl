namespace vg
{
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
		return _isHasObject(pTarget, m_mapPVisualObjects);
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
		return _isHasObject(pTarget, m_mapPCameras);
	}

	template <SpaceType SPACE_TYPE>
	void Scene<SPACE_TYPE>::addCamera(CameraType *pTarget, CameraType *pParent)
	{
		_addObject(pTarget
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
		_removeObject(pTarget
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
		return _isHasObject(pTarget, m_mapPCameras);
	}

	template <SpaceType SPACE_TYPE>
	void Scene<SPACE_TYPE>::addLight(LightType *pTarget, LightType *pParent)
	{
		_addObject(pTarget
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
		_removeObject(pTarget
			, m_arrPLights
			, m_mapPLights
			, m_mapTransformIdToLights
		);
	}

	template <SpaceType SPACE_TYPE>
	void Scene<SPACE_TYPE>::_addVisualObject(VisualObjectType *pTarget
		, VisualObjectType *pParent)
	{
		_addObject(pTarget
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
		_removeObject(pTarget
			, m_arrPVisualObjects
			, m_mapPVisualObjects
			, m_mapTransformIdToVisualObjects
		);
	}

	template <SpaceType SPACE_TYPE>
	template <typename T>
	Bool32 Scene<SPACE_TYPE>::_isHasObject(const T *pTarget, const std::unordered_map<InstanceID, T *> &map) const
	{
		return map.find(pTarget->getID()) != map.cend();
	}

	template <SpaceType SPACE_TYPE>
	template <typename T>
	void Scene<SPACE_TYPE>::_addObject(T *pTarget
		, std::vector<T *> &arr
		, std::unordered_map<InstanceID, T *> &map
		, std::unordered_map<InstanceID, T *> &mapTransformToObjs
		, TransformType *root
		, T *pParent
	)
	{

#ifdef DEBUG
		//chilren of target
		if (pTarget->getTransform()->getChildCount())
			throw std::invalid_argument("Target should not own chilren.");
#endif // DEBUG

		if (_isHasObject(pTarget, map)) return;
		arr.push_back(pTarget);
		map[pTarget->getID()] = pTarget;
		mapTransformToObjs[pTarget->getTransform()->getID()] = pTarget;

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
	template <typename T>
	void Scene<SPACE_TYPE>::_removeObject(T *pTarget
		, std::vector<T *> &arr
		, std::unordered_map<InstanceID, T *> &map
		, std::unordered_map<InstanceID, T *> &mapTransformToObjs
	)
	{
		if (_isHasObject(pTarget, map) == VG_FALSE) return;
		for (auto iter = arr.begin(); iter != arr.end(); ++iter)
		{
			if (*iter == pTarget)
			{
				arr.erase(iter);
				break;
			}
		}
		map.erase(pTarget->getID());
		mapTransformToObjs.erase(pTarget->getTransform()->getID());
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
} //namespace kgs