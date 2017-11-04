namespace kgs
{
	template <SpaceType SPACE_TYPE>
	Scene<SPACE_TYPE>::Scene()
		: BaseScene()
	{

	}

	template <SpaceType SPACE_TYPE>
	uint32_t Scene<SPACE_TYPE>::getVisualObjectCount()
	{
		return static_cast<uint32_t>(m_arrPVisualObjects.size());
	}

	template <SpaceType SPACE_TYPE>
	const std::shared_ptr<typename Scene<SPACE_TYPE>::VisualObjectType> Scene<SPACE_TYPE>::getVisualObjectWithIndex(uint32_t index) const
	{
		return m_arrPVisualObjects[index];
	}

	template <SpaceType SPACE_TYPE>
	Bool32 Scene<SPACE_TYPE>::isHasVisualObject(const std::shared_ptr<VisualObjectType> pTarget) const
	{
		return _isHasObject(pTarget, m_arrPVisualObjects, m_mapPVisualObjects);
	}

	template <SpaceType SPACE_TYPE>
	void Scene<SPACE_TYPE>::addVisualObject(const std::shared_ptr<VisualObjectType> pTarget
		, const std::shared_ptr<VisualObjectType> pParent)
	{
		_addVisualObject(pTarget, pParent);
	}

	template <SpaceType SPACE_TYPE>
	void Scene<SPACE_TYPE>::removeVisualObject(const std::shared_ptr<VisualObjectType> pTarget)
	{
		_removeVisualObject(pTarget);
	}

	template <SpaceType SPACE_TYPE>
	uint32_t Scene<SPACE_TYPE>::getCameraCount()
	{
		return static_cast<uint32_t>(m_arrPCameras.size());
	}

	template <SpaceType SPACE_TYPE>
	const std::shared_ptr<typename Scene<SPACE_TYPE>::CameraType> Scene<SPACE_TYPE>::getCameraWithIndex(uint32_t index) const
	{
		return m_arrPCameras[index];
	}

	template <SpaceType SPACE_TYPE>
	Bool32 Scene<SPACE_TYPE>::isHasCamera(const std::shared_ptr<CameraType> pTarget) const
	{
		return _isHasObject(pTarget, m_arrPCameras, m_mapPCameras);
	}

	template <SpaceType SPACE_TYPE>
	void Scene<SPACE_TYPE>::addCamera(const std::shared_ptr<CameraType> pTarget
		, const std::shared_ptr<CameraType> pParent)
	{
		_addObject(pTarget, m_arrPCameras, m_mapPCameras, pParent);
	}

	template <SpaceType SPACE_TYPE>
	void Scene<SPACE_TYPE>::removeCamera(const std::shared_ptr<CameraType> pTarget)
	{
		_removeObject(pTarget, m_arrPCameras, m_mapPCameras);
	}

	template <SpaceType SPACE_TYPE>
	uint32_t Scene<SPACE_TYPE>::getLightCount()
	{
		return static_cast<uint32_t>(m_arrPLights.size());
	}

	template <SpaceType SPACE_TYPE>
	const std::shared_ptr<typename Scene<SPACE_TYPE>::LightType> Scene<SPACE_TYPE>::getLightWithIndex(uint32_t index) const
	{
		return m_arrPLights[index];
	}

	template <SpaceType SPACE_TYPE>
	Bool32 Scene<SPACE_TYPE>::isHasLight(const std::shared_ptr<LightType> pTarget) const
	{
		return _isHasObject(pTarget, m_arrPCameras, m_mapPCameras);
	}

	template <SpaceType SPACE_TYPE>
	void Scene<SPACE_TYPE>::addLight(const std::shared_ptr<LightType> pTarget
		, const std::shared_ptr<LightType> pParent)
	{
		_addObject(pTarget, m_arrPLights, m_mapPLights, pParent);
	}

	template <SpaceType SPACE_TYPE>
	void Scene<SPACE_TYPE>::removeLight(const std::shared_ptr<LightType> pTarget)
	{
		_removeObject(pTarget, m_arrPLights, m_mapPLights);
	}

	template <SpaceType SPACE_TYPE>
	void Scene<SPACE_TYPE>::_addVisualObject(const std::shared_ptr<VisualObjectType> pTarget
		, const std::shared_ptr<VisualObjectType> pParent)
	{
		_addObject(pTarget, m_arrPVisualObjects, m_mapPVisualObjects, pParent);
	}

	template <SpaceType SPACE_TYPE>
	void Scene<SPACE_TYPE>::_removeVisualObject(const std::shared_ptr<VisualObjectType> pTarget)
	{
		_removeObject(pTarget, m_arrPVisualObjects, m_mapPVisualObjects);
	}

	template <SpaceType SPACE_TYPE>
	template <typename T>
	Bool32 Scene<SPACE_TYPE>::_isHasObject(const std::shared_ptr<T> &pTarget
		, const std::vector<std::shared_ptr<T>> &arr
		, const std::unordered_map<InstanceID, std::shared_ptr<T>> &map) const
	{
		return map.find(pTarget->getID()) != map.cend();
	}

	template <SpaceType SPACE_TYPE>
	template <typename T>
	void Scene<SPACE_TYPE>::_addObject(const std::shared_ptr<T> &pTarget
		, std::vector<std::shared_ptr<T>> &arr
		, std::unordered_map<InstanceID, std::shared_ptr<T>> &map
		, const std::shared_ptr<T> &pParent
	)
	{
		if (_isHasObject(pTarget, arr, map)) return;
		arr.push_back(pTarget);
		map[pTarget->getID()] = pTarget;

		//construct hierarchy data.
		if (pParent != nullptr)
		{
			pTarget->getTransform()->setParent(dynamic_cast<TransformType *>(pParent->getTransform().get()));
		}
		else
		{
			pTarget->getTransform()->setParent(nullptr);
		}
	}

	template <SpaceType SPACE_TYPE>
	template <typename T>
	void Scene<SPACE_TYPE>::_removeObject(const std::shared_ptr<T> &pTarget
		, std::vector<std::shared_ptr<T>> &arr
		, std::unordered_map<InstanceID, std::shared_ptr<T>> &map)
	{
		if (_isHasObject(pTarget) == KGS_FALSE) return;
		std::remove(arr.begin(), arr.end(), pTarget);
		map.erase(pTarget->getID());
		//clear hierarchy data.
		pTarget->getTransform()->setParent(nullptr);
		pTarget->getTransform()->detachChildren();
	}
} //namespace kgs