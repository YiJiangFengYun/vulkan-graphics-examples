#ifndef VG_SCENE_H
#define VG_SCENE_H

#include <vector>
#include <unordered_map>
#include "graphics/util/util.hpp"
#include "graphics/scene/object.hpp"
#include "graphics/scene/visual_object.hpp"
#include "graphics/scene/camera.hpp"
#include "graphics/scene/light.hpp"

namespace vg
{
	class BaseScene : public Base
	{
	public:
		BaseScene();
		SpaceType getSpaceType() const;
		Bool32 getIsRightHand() const;
		Bool32 getIsLeftHand() const;
		void setIsRightHand(Bool32 isRightHand);
		void setIsLeftHand(Bool32 isLeftHand);
	protected:
	    SpaceType m_spaceType;
		Bool32 m_isRightHand;
	};

	template <SpaceType SPACE_TYPE>
	class Scene : public BaseScene
	{
	public:
		using ObjectType = Object<SPACE_TYPE>;
		using VisualObjectType = VisualObject<SPACE_TYPE>;
		using CameraType = Camera<SPACE_TYPE>;
		using LightType = Light<SPACE_TYPE>;
		using TransformType = Transform<SPACE_TYPE>;

		const std::shared_ptr<TransformType> pRootTransformForVisualObject;
		const std::shared_ptr<TransformType> pRootTransformForCamera;
		const std::shared_ptr<TransformType> pRootTransformForLight;

		Scene();

		uint32_t getVisualObjectCount() const;
		const std::shared_ptr<VisualObjectType> &getVisualObjectWithIndex(uint32_t index) const;
		const std::shared_ptr<VisualObjectType> &getVisualObjectWithTransform(std::shared_ptr<TransformType> pTransform) const;
		const std::shared_ptr<VisualObjectType> &getVisualObjectWithTransform(TransformType *pTransform) const;
		Bool32 isHasVisualObject(const std::shared_ptr<VisualObjectType> pTarget) const;
		void addVisualObject(const std::shared_ptr<VisualObjectType> pTarget
			, const std::shared_ptr<VisualObjectType> pParent = nullptr);
		void removeVisualObject(const std::shared_ptr<VisualObjectType> pTarget);

		uint32_t getCameraCount() const;
		const std::shared_ptr<CameraType> &getCameraWithIndex(uint32_t index) const;
		const std::shared_ptr<CameraType> &getCameraWithTransform(std::shared_ptr<TransformType> pTransform) const;
		const std::shared_ptr<CameraType> &getCameraWithTransform(TransformType *pTransform) const;
		Bool32 isHasCamera(const std::shared_ptr<CameraType> pTarget) const;
		void addCamera(const std::shared_ptr<CameraType> pTarget
			, const std::shared_ptr<CameraType> pParent = nullptr);
		void removeCamera(const std::shared_ptr<CameraType> pTarget);

		uint32_t getLightCount() const;
		const std::shared_ptr<LightType> &getLightWithIndex(uint32_t index) const;
		const std::shared_ptr<LightType> &getLightWithTransform(std::shared_ptr<TransformType> pTransform) const;
		const std::shared_ptr<LightType> &getLightWithTransform(TransformType *pTransform) const;
		Bool32 isHasLight(const std::shared_ptr<LightType> pTarget) const;
		void addLight(const std::shared_ptr<LightType> pTarget
			, const std::shared_ptr<LightType> pParent = nullptr);
		void removeLight(const std::shared_ptr<LightType> pTarget);


	protected:

		//aggregations
		std::vector<std::shared_ptr<VisualObjectType>> m_arrPVisualObjects;
		std::unordered_map<InstanceID, std::shared_ptr<VisualObjectType>> m_mapPVisualObjects;
		std::unordered_map<InstanceID, std::shared_ptr<VisualObjectType>> m_mapTransformIdToVisualObjects;
		std::vector<std::shared_ptr<CameraType>> m_arrPCameras;
		std::unordered_map<InstanceID, std::shared_ptr<CameraType>> m_mapPCameras;
		std::unordered_map<InstanceID, std::shared_ptr<CameraType>> m_mapTransformIdToCameras;
		std::vector<std::shared_ptr<LightType>> m_arrPLights;
		std::unordered_map<InstanceID, std::shared_ptr<LightType>> m_mapPLights;
		std::unordered_map<InstanceID, std::shared_ptr<LightType>> m_mapTransformIdToLights;

		virtual void _addVisualObject(const std::shared_ptr<VisualObjectType> pTarget
			, const std::shared_ptr<VisualObjectType> pParent);
		virtual void _removeVisualObject(const std::shared_ptr<VisualObjectType> pTarget);
	private:
		template <typename T>
		inline Bool32 _isHasObject(const std::shared_ptr<T> &pTarget
			, const std::unordered_map<InstanceID, std::shared_ptr<T>> &map
		) const;

		template <typename T>
		inline void _addObject(const std::shared_ptr<T> &pTarget
			, std::vector<std::shared_ptr<T>> &arr
			, std::unordered_map<InstanceID, std::shared_ptr<T>> &map
			, std::unordered_map<InstanceID, std::shared_ptr<T>> &mapTransformToObjs
			, const std::shared_ptr<TransformType> &root
			, const std::shared_ptr<T> &pParent = nullptr
		);

		template <typename T>
		inline void _removeObject(const std::shared_ptr<T> &pTarget
			, std::vector<std::shared_ptr<T>> &arr
			, std::unordered_map<InstanceID, std::shared_ptr<T>> &map
			, std::unordered_map<InstanceID, std::shared_ptr<T>> &mapTransformToObjs
		);
	};
} //namespace kgs

#include "graphics/scene/scene.inl"

#endif // !VG_SCENE_H
