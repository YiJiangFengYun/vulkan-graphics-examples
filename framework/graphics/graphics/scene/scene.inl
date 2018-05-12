namespace vg
{
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
		if (_isHasObject(pTarget, map)) return;		
		_addObjectSetObjectOnly(dynamic_cast<ObjectType *>(pTarget), root, pParent);
		_addObjectAddSceneDataOnly(pTarget, arr, map, mapTransformToObjs);
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
		_removeObjectDeleteSceneDataOnly(pTarget, arr, map, mapTransformToObjs);
		_removeObjectSetObjectOnly(dynamic_cast<ObjectType *>(pTarget));
	}

    template <SpaceType SPACE_TYPE>
	template <typename T>
	void Scene<SPACE_TYPE>::_addObjectAddSceneDataOnly(T *pTarget
	    , std::vector<T *> &arr
		, std::unordered_map<InstanceID, T *> &map
		, std::unordered_map<InstanceID, T *> &mapTransformToObjs
		)
	{
		arr.push_back(pTarget);
		map[pTarget->getID()] = pTarget;
		mapTransformToObjs[pTarget->getTransform()->getID()] = pTarget;
	}

    template <SpaceType SPACE_TYPE>
	template <typename T>
	void Scene<SPACE_TYPE>::_removeObjectDeleteSceneDataOnly(T *pTarget
	    , std::vector<T *> &arr
		, std::unordered_map<InstanceID, T *> &map
		, std::unordered_map<InstanceID, T *> &mapTransformToObjs
		)
    {
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
	}
} //namespace kgs