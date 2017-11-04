#ifndef KGS_SCENE_H
#define KGS_SCENE_H

#include <vector>
#include <unordered_map>
#include "graphics/util/util.hpp"
#include "graphics/scene/object.hpp"
#include "graphics/scene/visual_object.hpp"
#include "graphics/scene/camera.hpp"
#include "graphics/scene/light.hpp"

namespace kgs
{
	class BaseScene : public Base
	{
	public:
		BaseScene();
	protected:

	};

	template <SpaceType SPACE_TYPE>
	class Scene : public BaseScene
	{
	public:
		typedef Object<SPACE_TYPE> ObjectType;
		typedef VisualObject<SPACE_TYPE> VisualObjectType;
		typedef Camera<SPACE_TYPE> CameraType;
		typedef Light<SPACE_TYPE> LightType;
		typedef Transform<SPACE_TYPE> TransformType;

		Scene();

		uint32_t getVisualObjectCount();
		const std::shared_ptr<VisualObjectType> getVisualObjectWithIndex(uint32_t index) const;
		Bool32 isHasVisualObject(const std::shared_ptr<VisualObjectType> pTarget) const;
		void addVisualObject(const std::shared_ptr<VisualObjectType> pTarget
			, const std::shared_ptr<VisualObjectType> pParent = nullptr);
		void removeVisualObject(const std::shared_ptr<VisualObjectType> pTarget);

		uint32_t getCameraCount();
		const std::shared_ptr<CameraType> getCameraWithIndex(uint32_t index) const;
		Bool32 isHasCamera(const std::shared_ptr<CameraType> pTarget) const;
		void addCamera(const std::shared_ptr<CameraType> pTarget
			, const std::shared_ptr<CameraType> pParent = nullptr);
		void removeCamera(const std::shared_ptr<CameraType> pTarget);

		uint32_t getLightCount();
		const std::shared_ptr<LightType> getLightWithIndex(uint32_t index) const;
		Bool32 isHasLight(const std::shared_ptr<LightType> pTarget) const;
		void addLight(const std::shared_ptr<LightType> pTarget
			, const std::shared_ptr<LightType> pParent = nullptr);
		void removeLight(const std::shared_ptr<LightType> pTarget);


	protected:

		//aggregations
		std::vector<std::shared_ptr<VisualObjectType>> m_arrPVisualObjects;
		std::unordered_map<InstanceID, std::shared_ptr<VisualObjectType>> m_mapPVisualObjects;
		std::vector<std::shared_ptr<CameraType>> m_arrPCameras;
		std::unordered_map<InstanceID, std::shared_ptr<CameraType>> m_mapPCameras;
		std::vector<std::shared_ptr<LightType>> m_arrPLights;
		std::unordered_map<InstanceID, std::shared_ptr<LightType>> m_mapPLights;

		template <typename T>
		inline Bool32 _isHasObject(const std::shared_ptr<T> &pTarget
			, const std::vector<std::shared_ptr<T>> &arr
			, const std::unordered_map<InstanceID, std::shared_ptr<T>> &map
		) const;

		template <typename T>
		inline void _addObject(const std::shared_ptr<T> &pTarget
			, std::vector<std::shared_ptr<T>> &arr
			, std::unordered_map<InstanceID, std::shared_ptr<T>> &map
			, const std::shared_ptr<T> &pParent = nullptr
		);

		template <typename T>
		inline void _removeObject(const std::shared_ptr<T> &pTarget
			, std::vector<std::shared_ptr<T>> &arr
			, std::unordered_map<InstanceID, std::shared_ptr<T>> &map
		);
	};
} //namespace kgs

#include "graphics/scene/scene.inl"

#endif // !KGS_SCENE_H
