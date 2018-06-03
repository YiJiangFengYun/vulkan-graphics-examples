namespace vg
{
    template <typename T>
    Bool32 BaseScene::_isHasObject(const T *pTarget, const std::unordered_map<InstanceID, T *> &map) const
    {
        return map.find(pTarget->getID()) != map.cend();
    }

    template <typename T>
    void BaseScene::_addObject(T *pTarget
        , std::vector<T *> &arr
        , std::unordered_map<InstanceID, T *> &map
        , std::unordered_map<InstanceID, T *> &mapTransformToObjs
        , BaseTransform *root
        , T *pParent
    )
    {
        if (_isHasObject(pTarget, map)) return;        
        _addObjectSetObjectOnly(pTarget, root, pParent);
        _addObjectAddSceneDataOnly(pTarget, arr, map, mapTransformToObjs);
    }

    template <typename T>
    void BaseScene::_removeObject(T *pTarget
        , std::vector<T *> &arr
        , std::unordered_map<InstanceID, T *> &map
        , std::unordered_map<InstanceID, T *> &mapTransformToObjs
    )
    {
        if (_isHasObject(pTarget, map) == VG_FALSE) return;
        _removeObjectDeleteSceneDataOnly(pTarget, arr, map, mapTransformToObjs);
        _removeObjectSetObjectOnly(pTarget);
    }

    template <typename T>
    void BaseScene::_addObjectAddSceneDataOnly(T *pTarget
        , std::vector<T *> &arr
        , std::unordered_map<InstanceID, T *> &map
        , std::unordered_map<InstanceID, T *> &mapTransformToObjs
        )
    {
        arr.push_back(pTarget);
        map[pTarget->getID()] = pTarget;
        mapTransformToObjs[pTarget->getTransform()->getID()] = pTarget;
    }

    template <typename T>
    void BaseScene::_removeObjectDeleteSceneDataOnly(T *pTarget
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